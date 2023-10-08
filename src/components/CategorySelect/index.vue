<template>
  <div>
    <el-form :inline="true" class="demo-form-inline" :model="cForm">
      <el-form-item label="First Category">
        <!-- 这里的@change是一个自定义事件 -->
        <el-select
          placeholder="Please Choose"
          v-model="cForm.category1Id"
          @change="getCategory2List"
          :disabled="show"
        >
          <!-- 二级分类需要一级分类的id，而v-model获取的是value值，所以将value值置为id -->
          <el-option
            :label="c1.name"
            :value="c1.id"
            v-for="c1 in list1"
            :key="c1.id"
          ></el-option>
        </el-select>
      </el-form-item>
      <el-form-item label="Second Category">
        <el-select
          placeholder="Please Choose"
          v-model="cForm.category2Id"
          @change="getCategory3List"
          :disabled="show"
        >
          <el-option
            :label="c2.name"
            :value="c2.id"
            v-for="c2 in list2"
            :key="c2.id"
          ></el-option>
        </el-select>
      </el-form-item>
      <el-form-item label="Third Category">
        <el-select
          placeholder="Please Choose"
          v-model="cForm.category3Id"
          @change="getAttrInfoLiat"
          :disabled="show"
        >
          <el-option
            :label="c3.name"
            :value="c3.id"
            v-for="c3 in list3"
            :key="c3.id"
          ></el-option>
        </el-select>
      </el-form-item>
    </el-form>
  </div>
</template>

<script>
export default {
  name: 'CategorySelect',
  data() {
    return {
      //一级分类数据
      list1: [],
      //二级分类数据
      list2: [],
      //三级分类数据
      list3: [],
      //收集分类id
      cForm: {
        category1Id: '',
        category2Id: '',
        category3Id: '',
      },
    }
  },
  props: ['show'],
  mounted() {
    //组件挂载完毕，发请求获取一级分类信息
    this.getCategory1List()
  },
  methods: {
    //获取一级分类
    async getCategory1List() {
      let result = await this.$API.attr.reqCategory1List()
      if (result.code == 200) {
        this.list1 = result.data
      } else {
        this.$message.error('Fail to obtain the category')
      }
    },
    //获取二级分类
    async getCategory2List() {
      const { category1Id } = this.cForm
      this.cForm.category2Id = ''
      this.cForm.category3Id = ''
      this.list3 = []
      this.$emit('getCategoryId', { categoryId: category1Id, level: 1 })
      let result = await this.$API.attr.reqCategory2List(category1Id)
      if (result.code == 200) {
        this.list2 = result.data
      } else {
        this.$message.error('Fail to obtain the category')
      }
    },
    //获取三级分类
    async getCategory3List() {
      const { category2Id } = this.cForm
      this.cForm.category3Id = ''
      this.$emit('getCategoryId', { categoryId: category2Id, level: 2 })
      let result = await this.$API.attr.reqCategory3List(category2Id)
      if (result.code == 200) {
        this.list3 = result.data
      } else {
        this.$message.error('Fail to obtain the category')
      }
    },
    async getAttrInfoLiat() {
      const { category3Id } = this.cForm
      this.$emit('getCategoryId', { categoryId: category3Id, level: 3 })
    },
  },
}
</script>

<style>
</style>