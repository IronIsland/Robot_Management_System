import mockRequest from '@/utils/mockRequest'
const actions = {
    //发请求获取模拟mock数据
    async getData({ commit }) {
        let result = await mockRequest.get('home/list')
        if (result.code == 20000) {
            commit('GETDATA', result.data)
        }
    }
};
const mutations = {
    GETDATA(state, list) {
        state.list = list
    }
};
const state = {
    list: {}
};
const getters = {};
export default {
    actions,
    mutations,
    state,
    getters
}