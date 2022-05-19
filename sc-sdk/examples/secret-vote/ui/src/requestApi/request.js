import axios from 'axios'
import router from '../router'
import { getToken, removeToken, removeUserInfo } from '../utils/auth'
import { Message } from 'element-ui';

const axiosRequest = axios.create({
    timeout: 5000
});

axiosRequest.interceptors.request.use(config => {
    if (getToken()) {
        config.headers['X-CSRFToken'] = getToken();
    }
    return config;
}, error => {
    Promise.reject(error);
})

axiosRequest.interceptors.response.use(response => {
    return response;
}, error => {
    const { status } = error.response;
    if (status === 403) {
        return Promise.reject(error);
    } else if (status === 401) {
        removeToken();
        removeUserInfo();
        router.push('/login');
    } else {
        removeToken();
        removeUserInfo();
        router.push('/login');
        Message({
            type: 'error',
            showClose: true,
            duration: 0,
            message: '未知错误',
            offset: 84
        })
    }
    return Promise.reject(error);
})

// 登录
export function doLogin(data) {
    return axiosRequest({
        url: '/user/login',
        method: 'post',
        data
    })
}

// 退出
export function doLogout() {
    return axiosRequest({
        url: '/user/logout',
        method: 'delete'
    })
}

// 获得投票数据
export function getVoteData() {
    return axiosRequest({
        url: '/vote_actions/show_vote_info_list',
        method: 'get'
    })
}

// 投票
export function doVote(data) {
    return axiosRequest({
        url: '/vote_actions/vote',
        method: 'post',
        data
    })
}