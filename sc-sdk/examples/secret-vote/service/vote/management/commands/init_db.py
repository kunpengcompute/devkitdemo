# -*- encoding: utf-8 -*-

"""
Copyright: Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
Create: 2022-05-16
Content: vote_system customize command
"""
import logging
import os
import shutil
from ctypes import cdll, c_char_p, c_int

from django.contrib.sessions.models import Session
from django.core.management import BaseCommand

from vote.models import User, VoteOption
from vote.utils import CA_PATH, KEY_DIRECTORY_PATH, ROOT_KEY_STORAGE_PATH, USER_PUBLIC_KEY_DIRECTORY_PATH, \
    PRIVATE_KEY_PATH

LOGGER = logging.getLogger('vote')


def clean_data():
    """ Clean data and initialize necessary directory """ 
    if os.path.exists(KEY_DIRECTORY_PATH):
        shutil.rmtree(KEY_DIRECTORY_PATH)
    if os.path.exists(PRIVATE_KEY_PATH):
        shutil.rmtree(PRIVATE_KEY_PATH)

    os.makedirs(KEY_DIRECTORY_PATH)
    os.makedirs(PRIVATE_KEY_PATH)
    os.makedirs(os.path.join(KEY_DIRECTORY_PATH, 'root_pub_key'))
    os.makedirs(USER_PUBLIC_KEY_DIRECTORY_PATH)


def clear_db():
    """ Clear all related database data """
    User.objects.all().delete()
    VoteOption.objects.all().delete()
    Session.objects.all().delete()
    LOGGER.info('Clear database successfully')


def init_user_db():
    """ Initialize user database """
    for user_number in range(10):
        user = User()
        user.id = user_number
        user.set_username('user_0' + str(user_number))
        user.set_password('123456789')
        user.save()
    LOGGER.info('Initialize user data successfully')


def init_vote_option_db():
    """ Initialize vote option database """
    vote_option_msg_list = ['非常满意', '很满意', '一般', '很不满意', '非常不满意']
    for vote_id in range(5):
        vote_opt = VoteOption()
        vote_opt.id = vote_id
        vote_opt.set_option_id(vote_id)
        vote_opt.set_vote_opt_message(vote_option_msg_list[vote_id])
        vote_opt.save()
    LOGGER.info('Initialize vote data successfully')


def get_root_pub_key():
    """ Get root key from CA """
    # Call CA method to get the root public key
    ca = cdll.LoadLibrary(CA_PATH)
    ca.CreateRootPubKey.argtypes = [c_char_p, c_int]
    ca.CreateRootPubKey.restype = c_int
    root_key_path = ROOT_KEY_STORAGE_PATH.encode()
    result = ca.CreateRootPubKey(root_key_path, len(root_key_path))
    if result == 0:
        LOGGER.info('Create root key successfully')
    else:
        LOGGER.error('Failed to call CA method to generate root key')


class Command(BaseCommand):
    """ Custom command to initialize the database """
    def handle(self, *args, **options):
        clean_data()
        clear_db()
        init_user_db()
        init_vote_option_db()
        get_root_pub_key()
        LOGGER.info('All data initialized successfully')
        