import { login, logout, getInfo } from '@/api/user'
import { getToken, setToken, removeToken } from '@/utils/auth'
import { resetRouter, asyncRoutes, anyRoutes, constantRoutes } from '@/router'
import router from '@/router'
import cloneDeep from 'lodash/cloneDeep'

const getDefaultState = () => {
  return {
    //获取Token
    token: getToken(),
    //储存用户名
    name: '',
    //存储用户头像
    avatar: '',
    //菜单标志
    routes: [],
    //按钮标记
    buttons: [],
    //用户角色
    roles: [],
    //已有异步路由与服务器标记信息对比最终展示的路由
    resultAsyncRoutes: [],
    //用户展示的所有路由
    resultAllRoutes: []
  }
}

const state = getDefaultState()

const mutations = {
  //重置state
  RESET_STATE: (state) => {
    Object.assign(state, getDefaultState())
  },
  //存储token
  SET_TOKEN: (state, token) => {
    state.token = token
  },
  //存储用户信息
  SET_USERINFO: (state, userInfo) => {
    //存储用户名
    state.name = userInfo.name
    //存储用户头像
    state.avatar = userInfo.avatar
    //菜单权限标志
    state.routes = userInfo.routes
    //按钮权限标志
    state.buttons = userInfo.buttons
    //用户角色
    state.roles = userInfo.roles
  },
  //最终路由
  SET_RESULTASYNCROUTES: (state, asyncRoutes) => {
    state.resultAsyncRoutes = asyncRoutes.concat(anyRoutes)
    state.resultAllRoutes = constantRoutes.concat(state.resultAsyncRoutes)
    router.addRoutes(state.resultAsyncRoutes)
  }
}

const actions = {
  // 处理登录业务
  async login({ commit }, userInfo) {
    //解构用户名与密码
    const { username, password } = userInfo
    //使用 async和await方式
    let result = await login({ username: username.trim(), password: password })
    if (result.code == 20000) {
      //vuex存储token
      commit('SET_TOKEN', result.data.token)
      //持久化本地存储token
      setToken(result.data.token)
      return 'ok'
    } else {
      return Promise.reject(new Error('faile'))
    }
    /*   模板提供的.then .catch方式
      return new Promise((resolve, reject) => {
        //向服务器请求数据
        login({ username: username.trim(), password: password })
          //请求成功
          .then(response => {
            // console.log(response);
            //将response解构赋值给data
            const { data } = response
            //派发mutations业务
            commit('SET_TOKEN', data.token)
            //本地保存token
            setToken(data.token)
            resolve()
          }).catch(error => {
            reject(error)
          })
      }) */
  },

  // 获取用户信息
  getInfo({ commit, state }) {
    return new Promise((resolve, reject) => {
      getInfo(state.token).then(response => {
        const { data } = response
        if (!data) {
          return reject('Fail to login in！')
        }
        commit('SET_USERINFO', data)
        commit('SET_RESULTASYNCROUTES', computedAsyncRoutes(cloneDeep(asyncRoutes), data.routes))
        resolve(data)
      }).catch(error => {
        reject(error)
      })
    })
  },

  // user logout
  logout({ commit, state }) {
    return new Promise((resolve, reject) => {
      logout(state.token).then(() => {
        removeToken() // must remove  token  first
        resetRouter()
        commit('RESET_STATE')
        resolve()
      }).catch(error => {
        reject(error)
      })
    })
  },

  // remove token
  resetToken({ commit }) {
    return new Promise(resolve => {
      removeToken() // must remove  token  first
      commit('RESET_STATE')
      resolve()
    })
  }
}

//服务器返回的路由标记与所有异步路由进行对比，结果为该用户最终能够跳转的路由
const computedAsyncRoutes = (asyncRoutes, routes) => {
  return asyncRoutes.filter(item => {
    //如果服务器返回数组拥有对应信息
    if (routes.includes(item.name)) {
      //递归
      if (item.children && item.children.length) {
        item.children = computedAsyncRoutes(item.children, routes)
      }
      return true
    }
  })
}
export default {
  namespaced: true,
  state,
  mutations,
  actions
}

