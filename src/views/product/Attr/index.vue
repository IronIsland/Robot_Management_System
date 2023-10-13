<template>
  <div>
    <el-card style="margin: 20px 0px">
      <CategorySelect
        @getCategoryId="getCategoryId"
        :show="!isShowTable"
      ></CategorySelect>
    </el-card>
    <el-card>
      <!-- 平台属性展示页 -->
      <div v-show="isShowTable">
        <el-button
          type="primary"
          icon="el-icon-plus"
          style="margin-bottom: 20px"
          :disabled="!category3Id"
          @click="addAttr"
          >Add the attribute</el-button
        >
        <!-- 表格展示平台属性 -->
        <el-table border style="width: 100%" :data="attrList">
          <el-table-column
            type="index"
            label="ID"
            width="80px"
            align="center"
          >
          </el-table-column>
          <el-table-column prop="attrName" label="AttrName" width="150px">
          </el-table-column>
          <el-table-column prop="prop" label="AttrValue" width="width">
            <template slot-scope="{ row }">
              <el-tag
                style="margin: 0 10px"
                type="success"
                v-for="attrValue in row.attrValueList"
                :key="attrValue.id"
                >{{ attrValue.valueName }}</el-tag
              >
            </template>
          </el-table-column>
          <el-table-column prop="prop" label="Operation" width="150px">
            <template slot-scope="{ row, $index }">
              <el-button
                style="margin-right: 20px"
                type="warning"
                icon="el-icon-edit"
                size="mini"
                @click="updateAttr(row)"
              ></el-button>
              <el-popconfirm
                :title="`Sure to delete(${row.attrName})?`"
                @onConfirm="deleteAttr(row)"
              >
                <el-button
                  slot="reference"
                  type="danger"
                  icon="el-icon-delete"
                  size="mini"
                  :ref="`popover-${$index}`"
                ></el-button>
              </el-popconfirm>
            </template>
          </el-table-column>
        </el-table>
      </div>
      <!-- 平台属性添加或修改 -->
      <div v-show="!isShowTable">
        <el-form ref="form" label-width="55px" :inline="true" :model="attrInfo">
          <el-form-item label="AttrName">
            <el-input
              placeholder="Please enter the attribute name"
              v-model="attrInfo.attrName"
            ></el-input>
          </el-form-item>
        </el-form>
        <el-button
          type="primary"
          icon="el-icon-plus"
          @click="addAttrValue"
          :disabled="!attrInfo.attrName"
          >Add the attribute value</el-button
        >
        <el-button @click="isShowTable = true">cancel</el-button>
        <!-- 添加表格 -->
        <el-table
          style="width: 100%; margin: 20px 0"
          border
          :data="attrInfo.attrValueList"
        >
          <el-table-column
            type="index"
            label="ID"
            width="80px"
            align="center"
          >
          </el-table-column>
          <el-table-column prop="prop" label="Attribute valueName" width="width">
            <template slot-scope="{ row, $index }">
              <el-input
                v-model="row.valueName"
                size="mini"
                placeholder="Please enter the Attribute valueName"
                v-if="row.flag"
                @blur="toLook(row, $index)"
                @keyup.native.enter="toLook(row, $index)"
                :ref="$index"
              ></el-input>
              <span v-else @click="toEdit(row, $index)">{{
                row.valueName
              }}</span>
            </template>
          </el-table-column>
          <el-table-column prop="prop" label="Operation" width="width">
            <!-- 问题描述：在element-ui table中使用Popconfirm或者Popover时，点击Popconfirm或Popover中的el-button无法弹出确认框
                解决方案：在el-popconfirm组件中添加 :ref="`popover-${$index}`"即可（指定唯一的ref） -->
            <template slot-scope="{ row, $index }">
              <el-popconfirm
                :title="`Sure to delete ${row.valueName}?`"
                @onConfirm="deleteAttrValue($index)"
              >
                <el-button
                  slot="reference"
                  type="danger"
                  icon="el-icon-delete"
                  size="mini"
                  :ref="`popover-${$index}`"
                ></el-button>
              </el-popconfirm>
            </template>
          </el-table-column>
        </el-table>
        <el-button
          type="primary"
          @click="addOrUpdateAttr"
          :disabled="attrInfo.attrValueList.length < 1"
          >Save</el-button
        >
        <el-button @click="isShowTable = true">Cancel</el-button>
      </div>
    </el-card>
  </div>
</template>

<script>
//按需引入lodash中的深拷贝
import cloneDeep from 'lodash/cloneDeep'
export default {
  name: 'Attr',
  data() {
    return {
      keyword: '',
      category1Id: '',
      category2Id: '',
      category3Id: '',
      attrList: [],
      isShowTable: true,
      //收集新增属性，修改属性
      attrInfo: {
        attrName: '', //属性名
        attrValueList: [
          //属性值
        ],
        //对象存储数据时无序的，不能在data中收集数据
        categoryId: 0, //三级分类id
        categoryLevel: 3,
      },
    }
  },
  methods: {
    //自定义事件回调，获取id
    getCategoryId({ categoryId, level }) {
      switch (level) {
        case 1:
          this.category1Id = categoryId
          this.category2Id = ''
          this.category3Id = ''
          break
        case 2:
          this.category2Id = categoryId
          this.category3Id = ''
          break
        case 3:
          this.category3Id = categoryId
          this.getAttrList()
          break
        default:
          break
      }
    },
    //发请求获取属性信息
    async getAttrList() {
      //获取id
      const { category1Id, category2Id, category3Id } = this
      let result = await this.$API.attr.reqAttrInfoList(
        category1Id,
        category2Id,
        category3Id
      )
      if (result.code == 200) {
        this.attrList = result.data
      }
    },
    //添加属性值按钮
    addAttrValue() {
      this.attrInfo.attrValueList.push({
        attrId: this.attrInfo.id, //属性名id
        valueName: '',
        flag: true,
      })
      this.$nextTick(() => {
        this.$refs[this.attrInfo.attrValueList.length - 1].focus()
      })
    },
    //修改属性按钮
    updateAttr(row) {
      this.isShowTable = false
      //由于数据结构较为复杂，因此需要使用深拷贝
      this.attrInfo = cloneDeep(row)
      //在修改某个属性时，加上flag标记
      this.attrInfo.attrValueList.forEach((item) => {
        //此处添加的flag不是响应式数据
        // item.flag = false
        /* 
        Vue.set
        向响应式对象中添加一个 property，并确保这个新 property 同样是响应式的，
        且触发视图更新。它必须用于向响应式对象上添加新 property，因为 Vue 无法探测普通的新增 property
        */
        //第一个参数：对象  第二个参数：新的响应式属性  第三个参数：新的属性的属性值
        this.$set(item, 'flag', false)
      })
    },
    //添加属性按钮
    addAttr() {
      this.isShowTable = false
      //重置添加页
      this.attrInfo = {
        attrName: '', //属性名
        attrValueList: [],
        categoryId: this.category3Id, //收集三级分类id
        categoryLevel: 3,
      }
    },
    //添加属性的编辑与查看模式修改
    toLook(row, index) {
      if (row.valueName.trim() == '') {
        this.attrInfo.attrValueList.splice(index, 1)
        this.$message.error('The Attribute value can not be empty')
      }
      //不能新增已有的属性
      let isRepat = this.attrInfo.attrValueList.some((item) => {
        if (row !== item) {
          return row.valueName == item.valueName
        }
      })
      if (isRepat) {
        this.$message({
          message: 'Attribute value duplicated',
          type: 'warning',
        })
        return
      }
      row.flag = false
    },
    //自动聚焦
    toEdit(row, index) {
      row.flag = true
      //获取input节点
      this.$nextTick(() => {
        this.$refs[index].focus()
      })
    },
    //删除属性值按钮
    deleteAttrValue(index) {
      //当前不需要发请求
      this.attrInfo.attrValueList.splice(index, 1)
    },
    //保存
    async addOrUpdateAttr() {
      //整理参数
      this.attrInfo.attrValueList = this.attrInfo.attrValueList.filter(
        (item) => {
          //过滤为空的属性值
          if (item.valueName != '') {
            delete item.flag
            return true
          }
        }
      )
      //发请求
      try {
        await this.$API.attr.reqAddOrUpdateAttr(this.attrInfo)
        this.$message({
          message: 'Saved',
          type: 'success',
        })
        //重新获取
        this.getAttrList()
        this.isShowTable = true
      } catch (error) {
        this.$message.error('Fail to save')
      }
    },
    //删除属性
    async deleteAttr(row) {
      const { id } = row
      let result = await this.$API.attr.reqDeleteAttr(id)
      if (result.code == 200) {
        this.$message({
          message: 'Success to delete',
          type: 'warning',
        })
        this.getAttrList()
      }
    },
  },
}
</script>

<style>
</style>