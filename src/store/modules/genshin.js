import { reqGetData } from "@/api/genshin"

const actions = {
    async getGenshinData({ commit }) {
        //发请求
        let result = await reqGetData()
        commit('GETGENSHINDATA', result.data)
    }
}
const mutations = {
    GETGENSHINDATA(state, record) {
        state.record = record
    }
}
const state = {
    record: []
}
export default {
    actions,
    mutations,
    state
}