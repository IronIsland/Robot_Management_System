<template>
  <div class="charts" ref="charts"></div>
</template>

<script>
export default {
  name: 'LineChart',
  props: ['activityRate'],
  watch: {
    activityRate() {
      this.brokenLine()
    },
  },
  mounted() {
    this.brokenLine()
  },
  methods: {
    //初始化echarts
    brokenLine() {
      if(this.myCharts){
        this.myCharts.dispose()
      }
      this.myCharts = this.$echarts.init(this.$refs.charts)
      this.myCharts.setOption({
        xAxis: {
          show: false,
          //最大值与最小值
          min: 0,
          max: 100,
        },
        yAxis: {
          show: false,
          type: 'category',
        },
        series: [
          {
            data: [this.activityRate],
            type: 'bar',
            barWidth: 10,
            showBackground: true,
            label: {
              show: true,
              formatter: '|',
              position: 'right',
            },
            color: '#5470c6',
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