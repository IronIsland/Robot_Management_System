import request from '@/utils/request'

//获取一级分类接口
export const reqCategory1List = () => request({
    url: '/admin/product/getCategory1',
    method: 'get'
})

//获取二级分类接口
export const reqCategory2List = (category1Id) => request({
    url: `/admin/product/getCategory2/${category1Id}`,
    method: 'get'
})

//获取三级分类接口
export const reqCategory3List = (category2Id) => request({
    url: `/admin/product/getCategory3/${category2Id}`,
    method: 'get'
})

//获取商品基本属性 
export const reqAttrInfoList = (category1Id, category2Id, category3Id) => request({
    url: `/admin/product/attrInfoList/${category1Id}/${category2Id}/${category3Id}`,
    method: 'get'
})

//添加属性与修改属性值
export const reqAddOrUpdateAttr = (data) => request({
    url: '/admin/product/saveAttrInfo',
    method: 'post',
    data
})

//删除属性
export const reqDeleteAttr = (attrId) => request({
    url: `/admin/product/deleteAttr/${attrId}`,
    method: 'delete'
})