const TOKEN_KEY = 'vote-token';
const USERINFO_KEY = 'vote-user-login';

export function setToken(token) {
    window.sessionStorage.setItem(TOKEN_KEY, token);
}

export function getToken() {
    return document.cookie.substring(10) || window.sessionStorage.getItem(TOKEN_KEY);
}

export function removeToken() {
    window.sessionStorage.removeItem(TOKEN_KEY);
}

export function setUserInfo(userInfo) {
    if (!userInfo) return;
    const userInfoStr = JSON.stringify(userInfo);
    window.localStorage.setItem(USERINFO_KEY, userInfoStr);
}

export function getUserInfo() {
    const userInfoStr = window.localStorage.getItem(USERINFO_KEY);
    if (userInfoStr) {
        return JSON.parse(userInfoStr);
    } else {
        return null;
    }
}

export function removeUserInfo() {
    window.localStorage.removeItem(USERINFO_KEY);
}