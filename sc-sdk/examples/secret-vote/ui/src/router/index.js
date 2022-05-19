import Vue from 'vue'
import VueRouter from 'vue-router'
import UserLogin from '../views/UserLogin.vue'

Vue.use(VueRouter)

const routes = [
  {
    path: '/',
    redirect: '/login'
  },
  {
    path: '/login',
    name: 'UserLogin',
    component: UserLogin
  },
  {
    path: '/vote',
    name: 'VoteView',
    component: () => import('../views/VoteView')
  },
  {
    path: '*',
    redirect: '/login'
  },
]

const router = new VueRouter({
  routes
})

export default router
