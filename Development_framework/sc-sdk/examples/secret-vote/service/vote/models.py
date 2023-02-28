# -*- encoding: utf-8 -*-

"""
Copyright: Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
Create: 2022-05-16
Content: Models for vote system
"""
from ctypes import Structure, c_char_p, c_int, POINTER

from django.contrib.auth.base_user import AbstractBaseUser, BaseUserManager
from django.contrib.auth.hashers import make_password
from django.db import models


class User(AbstractBaseUser):
    """ User model """
    username = models.CharField(max_length=32, unique=True, default="")
    public_key = models.CharField(max_length=128, default="")
    sign = models.BinaryField(max_length=256, default=b"")
    login_status = models.BooleanField(default=False)
    vote_status = models.BooleanField(default=False)

    USERNAME_FIELD = 'username'
    objects = BaseUserManager()

    def __str__(self):
        return self.username

    def set_username(self, username):
        self.username = username

    def get_username(self):
        return self.username

    def set_public_key(self, public_key):
        self.public_key = public_key

    def get_public_key(self):
        return self.public_key

    def set_sign(self, sign):
        self.sign = sign

    def get_sign(self):
        return self.sign
    
    def set_login_status(self, login_status):
        self.login_status = login_status

    def get_login_status(self):
        return self.login_status

    def set_vote_status(self, vote_status):
        self.vote_status = vote_status

    def get_vote_status(self):
        return self.vote_status

    def set_password(self, raw_password):
        # Save the password encrypted
        self.password = make_password(raw_password)


class VoteOption(models.Model):
    """ Vote option model """
    option_id = models.IntegerField(default=1, unique=True)
    vote_opt_message = models.CharField(max_length=32, unique=True)
    votes_number = models.IntegerField(default=0)

    VOTE_TITLE = "产品满意度调查"
    VOTE_SUB_TITLE = "请问您对本产品的使用体验如何？"

    def set_option_id(self, option_id):
        self.option_id = option_id

    def get_option_id(self):
        return self.option_id

    def set_vote_opt_message(self, vote_opt_message):
        self.vote_opt_message = vote_opt_message

    def get_vote_opt_message(self):
        return self.vote_opt_message

    def set_votes_number(self, votes_number):
        self.votes_number = votes_number

    def get_votes_number(self):
        return self.votes_number


class UserPubkey(Structure):
    """
    User public key structure
    Reduce the number of input parameters when calling the CA method
    """
    _fields_ = [
        ('username', c_char_p),
        ('usernameLen', c_int),
        ('pubkeyPath', c_char_p),
        ('pubkeyPathLen', c_int),
        ('sign', c_char_p),
        ('signLen', c_int)
    ]


class VoteInfo(Structure):
    """
    Vote info structure
    Reduce the number of input parameters when calling the CA method
    """
    _fields_ = [
        ('voteData', c_char_p),
        ('voteDataLen', c_int),
        ('voteRes', c_char_p),
        ('voteResLen', POINTER(c_int))
    ]
