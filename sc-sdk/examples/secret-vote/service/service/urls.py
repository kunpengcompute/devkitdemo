# -*- encoding: utf-8 -*-

"""
Copyright: Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
Create: 2022-05-16
Content: Vote system router
"""

from django.urls import path

from vote import views

urlpatterns = [
    path('user/login', views.login_view),
    path('user/logout', views.logout_view),
    path('vote_actions/show_vote_info_list', views.show_vote_info_list),
    path('vote_actions/vote', views.vote)
]
