# -*- encoding: utf-8 -*-

"""
Copyright: Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
Create: 2022-05-16
Content: User manager and vote module
"""

import logging

from django.contrib.auth import authenticate, logout
from django.http import JsonResponse
from rest_framework.decorators import api_view, permission_classes
from rest_framework.permissions import AllowAny

from vote import utils
from vote.models import User, VoteOption
from vote.utils import new_generate_response_body, login_user, check_if_user_login, assemble_vote_data, \
    send_vote_msg

LOGGER = logging.getLogger('vote')


@api_view(['POST'])
@permission_classes([AllowAny, ])
def login_view(request):
    """
    Process user login request
    :param request: HttpRequest object
    :return: Id and username
    """
    # Get username and password
    username = request.data.get("username", "")
    password = request.data.get("password", "")

    # Check username format
    LOGGER.info('Start check username format')
    if not utils.validate_username(username):
        LOGGER.error(f'Invalid username: {username}')
        return JsonResponse(status=403, data=new_generate_response_body('login_name_pwd_error'))

    # Start login
    user_auth = authenticate(request, username=username, password=password)
    if user_auth is None:
        LOGGER.error('Authenticate failed')
        return JsonResponse(status=403, data=new_generate_response_body('login_name_pwd_error'))
    del password

    # Construct token and send it to the front end
    return login_user(request, user_auth)


@api_view(['DELETE'])
@check_if_user_login()
def logout_view(request):
    """
    Process user logout request
    :param request: HttpRequest object
    :return: Logout message
    """
    user = User.objects.get(id=request.user.id)
    user.set_login_status(False)
    user.save()
    logout(request)
    LOGGER.info('Logout successfully')
    return JsonResponse(status=200, data=new_generate_response_body('logout_success'))


@api_view(['GET'])
@check_if_user_login()
def show_vote_info_list(request):
    """
    Process vote data and send data to the front end
    :param request: HttpRequest object
    :return: If user vote status is true, just return vote option list.
            If user vote status is false, return vote option list and specific vote statistics.
    """
    user_vote_status = User.objects.get(id=request.user.id).get_vote_status()
    vote_data = assemble_vote_data(user_vote_status)
    LOGGER.info('Query vote data successfully')
    return JsonResponse(status=200, data=new_generate_response_body('query_success', data=vote_data))


@api_view(['POST'])
@check_if_user_login()
def vote(request):
    """
    Send vote data to CA and return message to the front end
    :param request: HttpRequest object, contains vote id
    :return: Vote option list and specific vote statistics
    """
    # Check user vote status
    user = User.objects.get(id=request.user.id)
    if user.get_vote_status() is True:
        vote_data = assemble_vote_data(user_vote_status=True)
        return JsonResponse(status=403, data=new_generate_response_body('duplicate_vote', data=vote_data))

    # Check request arguments
    vote_id = request.data.get('vote_id', -1)
    if int(vote_id) < 0:
        LOGGER.error('Invalid argument: vote_id')
        return JsonResponse(status=403, data=new_generate_response_body('vote_failed'))

    # Send vote message to CA
    vote_result = send_vote_msg(user, int(vote_id))
    if vote_result == -1:
        LOGGER.error('Failed to call CA vote method')
        return JsonResponse(status=403, data=new_generate_response_body('vote_failed'))

    # Update vote result
    vote_option = VoteOption.objects.get(option_id=int(vote_result))
    vote_option.set_votes_number(vote_option.get_votes_number() + 1)
    vote_option.save()
    user.set_vote_status(True)
    user.save()

    vote_data = assemble_vote_data(user_vote_status=True)
    LOGGER.info('Vote successfully')
    return JsonResponse(status=200, data=new_generate_response_body('vote_success', data=vote_data))
