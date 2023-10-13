<template>
  <div>
    <el-card class="box-card">
      <!-- 头部 -->
      <div slot="header" class="clearfix">
        <!-- 头部左侧 -->
        <el-tabs class="tab" v-model="activeName" @tab-click="changeBar">
          <el-tab-pane label="Angular Velocity" name="Angular Velocity"></el-tab-pane>
        </el-tabs>
        <!-- 头部右侧 -->
        <div class="right">
          <a @click="setDate">Day</a>
          <a @click="setWeek">Week</a>
          <a @click="setMonth">Month</a>
          <a @click="setYear">Year</a>
          <el-date-picker class="date" size="small" v-model="date" type="daterange" start-placeholder="Begin Date"
            end-placeholder="End Date" value-format="yyyy-MM-dd" :default-time="['00:00:00', '23:59:59']">
          </el-date-picker>
        </div>
      </div>
      <!-- 内容区 -->
      <div>
        <el-row :gutter="10">
          <el-col :span="24">
            <!-- 柱状图容器 -->
            <div class="charts" ref="charts"></div>
          </el-col>

        </el-row>
      </div>
    </el-card>
  </div>
</template>

<script>
import { mapState } from 'vuex'
import echarts from 'echarts'
//引入dayjs
var dayjs = require('dayjs')
export default {
  name: 'Sale',
  data() {
    return {
      date: [],
      activeName: 'Angular Velocity',
    }
  },
  computed: {
    ...mapState({
      listState: (state) => state.home.list,
    }),
  },
  mounted() {
    this.myCharts = this.$echarts.init(this.$refs.charts)
    this.myCharts.setOption({
      title: {
        text: this.activeName + ' Record'
      },
      legend: {
        data: ['AV_X', 'AV_Y', 'AV_Z', 'AV_A_X', 'AV_A_Y','AV_A_Z']
      },
      //there is some problem with the tip, as the value here is not same as the graph 
      tooltip: {
        trigger: 'axis'
        
      },
      grid: {
        left: '3%',
        right: '4%',
        bottom: '3%',
        containLabel: true
      },
      toolbox: {
        feature: {
          saveAsImage: {}
        }
      },
      xAxis: {
        type: 'category',
        boundaryGap: false,
        data: this.listState.Axis
      },
      yAxis: {
        type: 'value'
      },
      series: [
        {
          name: 'AV_X',
          type: 'line',
          stack: 'Total',
          data: this.listState.Angular_Velocity_X,
        },
        {
          name: 'AV_Y',
          type: 'line',
          stack: 'Total',
          data: this.listState.Angular_Velocity_Y,

        },
        {
          name: 'AV_Z',
          type: 'line',
          stack: 'Total',
          data: this.listState.Angular_Velocity_Z,
        },
        {
          name: 'AV_A_X',
          type: 'line',
          stack: 'Total',
          data: this.listState.Angular_Acceleration_X,
        },
        {
          name: 'AV_A_Y',
          type: 'line',
          stack: 'Total',
          data: this.listState.Angular_Acceleration_Y,
        },
        {
          name: 'AV_A_Z',
          type: 'line',
          stack: 'Total',
          data: this.listState.Angular_Acceleration_Z,
        }

      ]

    })
  },
  methods: {
    //Day
    setDate() {
      const day = dayjs().format('YYYY-MM-DD')
      this.date = [day, day]
    },
    //Week
    setWeek() {
      const start = dayjs().day(0).format('YYYY-MM-DD')
      const end = dayjs().day(6).format('YYYY-MM-DD')
      this.date = [start, end]
    },
    //Month
    setMonth() {
      const start = dayjs().startOf('month').format('YYYY-MM-DD')
      const end = dayjs().endOf('month').format('YYYY-MM-DD')
      this.date = [start, end]
    },
    //Year
    setYear() {
      const start = dayjs().startOf('year').format('YYYY-MM-DD')
      const end = dayjs().endOf('year').format('YYYY-MM-DD')
      this.date = [start, end]
    },
  },
  watch: {
    listState() {
      this.myCharts.setOption({
        xAxis: [
          {
            data: this.listState.Axis,
          },
        ],
        series: [
          {
            name: 'AV_X',
            type: 'line',
            stack: 'Total',
            data: this.listState.Angular_Velocity_X,
          },
          {
            name: 'AV_Y',
            type: 'line',
            stack: 'Total',
            data: this.listState.Angular_Velocity_Y,

          },
          {
            name: 'AV_Z',
            type: 'line',
            stack: 'Total',
            data: this.listState.Angular_Velocity_Z,
          },
          {
            name: 'AV_A_X',
            type: 'line',
            stack: 'Total',
            data: this.listState.Angular_Acceleration_X,
          },
          {
            name: 'AV_A_Y',
            type: 'line',
            stack: 'Total',
            data: this.listState.Angular_Acceleration_Y,
          },
          {
            name: 'AV_A_Z',
            type: 'line',
            stack: 'Total',
            data: this.listState.Angular_Acceleration_Z,
          }

        ],
      })
    },
  },
}
</script>

<style scoped>
.box-card {
  margin-top: 10px;
}

div>>>.el-card__header {
  border-bottom: none;
}

.clearfix {
  position: relative;
  justify-content: space-between;
}

.tab {
  width: 100%;
}

.right {
  position: absolute;
  right: 0;
  top: 0;
}

.date {
  width: 250px;
  margin: 0 20px;
}

.right a {
  margin: 0 10px;
}

.charts {
  width: 100%;
  height: 300px;
}

ul {
  list-style: none;
  width: 100%;
  height: 300px;
  padding: 0px;
}

ul li {
  height: 8%;
  margin: 10px 0px;
}

.rindex {
  float: left;
  width: 20px;
  height: 20px;
  border-radius: 50%;
  background: black;
  color: white;
  text-align: center;
}

.rvalue {
  float: right;
}
</style>