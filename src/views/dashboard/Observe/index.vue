<template>
  <div>
    <el-row :gutter="10">
      <el-col :span="12">
        <Search :searchWord="listState.searchWord"></Search>
      </el-col>
      <el-col :span="12">
        <Category :saleRank="listState.saleRank"></Category>
      </el-col>
    </el-row>
  </div>
</template>

<script>
//import Search from './Search'
import Category from './Category'
import { mapState } from 'vuex'
export default {
  name: '',
  computed: {
    ...mapState({
      listState: (state) => state.home.list,
    }),
  },
  components: {
    //Search,
    Category,
  },
}
</script>

<style scoped>
</style>
