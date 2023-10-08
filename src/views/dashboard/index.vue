<template>
  <div class="dashboard-container">
<!--  <Card></Card> -->    
    <Sale></Sale>
    <Observe></Observe>
  </div>
</template>

<script>
import { mapGetters } from 'vuex'
//import Card from './Card'
import Sale from './Sale'
import Observe from './Observe'
export default {
  name: 'Dashboard',
  computed: {
    ...mapGetters(['name']),
  },
  components: { Sale, Observe },//如果要用，记得加上组件名称
  mounted(){
    this.$store.dispatch('getData')
  }
}
</script>

<style lang="scss" scoped>
</style>