import axios from 'axios'
import router from '../router'
import { getToken, removeToken, removeUserInfo } from '../utils/auth'
import { Message } from 'element-ui';

const axiosRequest = axios.create({
    timeout: 15 * 1000 // 单位ms
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
    if (error.response) {
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
    } else {
        dealWithFailOrCancel(error);
    }
    return Promise.reject(error);
})

function dealWithFailOrCancel(error) {
    const errMessage = error.message;
    if (errMessage.includes('Network Error')) {
        Message({
            type: 'error',
            showClose: true,
            duration: 0,
            message: '网络错误',
            offset: 84
        })
    } else if (errMessage.includes('timeout')) {
        Message({
            type: 'error',
            showClose: true,
            duration: 0,
            message: '请求超时',
            offset: 84
        })
    } else {
        Message({
            type: 'error',
            showClose: true,
            duration: 0,
            message: errMessage,
            offset: 84
        })
    }
}

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