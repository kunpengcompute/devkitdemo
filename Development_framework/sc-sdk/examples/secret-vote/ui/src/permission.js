import router from './router'
import { getToken } from './utils/auth'

const whiteList = ['/login']

router.beforeEach(async (to, from, next) => {
    const hasToken = getToken();
    if (hasToken) {
        next();
    } else {
        if (whiteList.indexOf(to.path) !== -1) {
            next();
        } else {
            next(`/login?redirect=${to.path}`);
        }
    }
})