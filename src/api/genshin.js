import request from '@/utils/mockRequest'

export const reqGetData = () => request({
    url: '/genshin/list',
    method: 'get'
})