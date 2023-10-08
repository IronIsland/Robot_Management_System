<template>
  <div class="charts" ref="charts"></div>
</template>

<script>
import { mapState } from 'vuex'
export default {
  name: 'LineChart',
  computed: {
    ...mapState({
      listState: (state) => state.home.list,
    }),
  },
  watch: {
    listState() {
      this.brokenLine()
    },
  },
  methods: {
    //初始化echarts
    brokenLine() {
      let myCharts = this.$echarts.init(this.$refs.charts)
      myCharts.setOption({
        xAxis: {
          //隐藏x轴
          show: false,
          type: 'category',
        },
        yAxis: {
          show: false,
        },
        //系列
        series: [
          {
            type: 'line',
            smooth: true,
            data: this.listState.visitTrend,
            //点的样式
            itemStyle: {
              opacity: 0,
            },
            //线条样式
            lineStyle: {
              color: 'purple',
            },
            //填充颜色
            areaStyle: {
              color: {
                type: 'linear',
                x: 0,
                y: 0,
                x2: 0,
                y2: 1,
                colorStops: [
                  {
                    offset: 0,
                    color: 'purple', // 0% 处的颜色
                  },
                  {
                    offset: 1,
                    color: '#ffffff', // 100% 处的颜色
                  },
                ],
                global: false, // 缺省为 false
              },
            },
          },
        ],
        //布局调试
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