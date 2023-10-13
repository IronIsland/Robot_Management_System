<template>
  <div>
    <el-row :gutter="10">
      <el-col :span="12">
        <Rolling :searchWord="listState.searchWord"></Rolling>
        <!-- <Rolling :speed="2" :direction="'left'"></Rolling> -->
      </el-col>
      <el-col :span="12">
        <Category :saleRank="listState.saleRank"></Category>
      </el-col>
    </el-row>
  </div>
</template>

<script>
import Rolling from './Rolling'
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
    Category,
    Rolling
  },
}
</script>

<style scoped></style>
