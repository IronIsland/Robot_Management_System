<template>
  <div class="charts" ref="charts"></div>
</template>

<script>
export default {
  name: 'LineChart',
  props: ['payTrend'],
  watch: {
    payTrend() {
      this.brokenLine()
    },
  },
  methods: {
    //初始化echarts
    brokenLine() {
      let myCharts = this.$echarts.init(this.$refs.charts)
      myCharts.setOption({
        xAxis: {
          type: 'category',
          show: false,
          // data: ['Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat', 'Sun'],
        },
        yAxis: {
          show: false,
          type: 'value',
        },
        series: [
          {
            data: this.payTrend,
            type: 'bar',
            color: 'red',
          },
        ],
        grid: {
          left: 0,
          top: 0,
          bottom: 0,
          right: 0,
        },
      })
    },
  },
}
</script>

<style scoped>
.charts {
  width: 100%;
  height: 100%;
}
</style>