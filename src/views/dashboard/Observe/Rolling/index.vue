<template>
    <div id="app">
        <div class="SwiperBox" @mouseenter="MouseFun('移入')" @mouseleave="MouseFun('移出')">
            <!-- 图片 -->
            <img :class="['imgCss', ShowImg == index ? 'ShowCss' : '']" :src="item.imgUrl" v-for="(item, index) in imgList"
                :key="index" />
            <!-- 左箭头按钮 -->
            <div class="leftBtn" @click="throttle(PrevFun)">&larr;</div>
            <!-- 右箭头按钮 -->
            <div class="rightBtn" @click="throttle(NextFun)">&rarr;</div>
            <!-- 下方指示点容器 -->
            <div class="instBox">
                <div v-for="(item, index) in imgList.length" :key="index" @click="ShowImg = index"
                    :class="['inst', ShowImg == index ? 'instActv' : '']">
                </div>
            </div>
        </div>
    </div>
</template>
  
<script>
export default {
    components: {},
    data() {
        return {

            imgList: [
                { imgUrl: require("@/assets/Circular_rolling/1.jpg") },
                { imgUrl: require("@/assets/Circular_rolling/2.jpg")},
                { imgUrl: require("@/assets/Circular_rolling/3.jpg") },
                { imgUrl: require("@/assets/Circular_rolling/4.jpg") },
                { imgUrl: require("@/assets/Circular_rolling/5.jpg") },

            ],
            ShowImg: 0,  // 表示当前显示的图片
            flag: true, // 用来节流防止重复点击
            start: null, // 自动执行下一张定时器
        };
    },
    mounted() {
        this.setTimeoFun()
    },
    methods: {
        // 这里定义一个鼠标移入移出事件，鼠标悬停时停止自动轮播，鼠标移出则重新执行自动轮播
        MouseFun(type) {// 停止定时器            // 重新执行定时器
            type == '移入' ? clearTimeout(this.start) : this.setTimeoFun()
        },
        setTimeoFun() {
            this.start = setInterval(() => {
                this.NextFun()
            }, 1500)
        },
        // 这里通过额外封装的节流函数触发 PrevFun() 和 NextFun(),以达到防止重复点击的效果
        throttle(fun) {
            if (this.flag) {
                this.flag = false;
                fun(); // 此为模板中传递进来的PrevFun()或NextFun()函数
                setTimeout(() => {
                    this.flag = true;
                }, 100); // 节流间隔时间
            }
        },
        // 上一张
        PrevFun() {
            if (this.ShowImg !== 0) {
                this.ShowImg--
            } else {
                this.ShowImg = this.imgList.length - 1
            }
        },
        // 下一张
        NextFun() {
            if (this.ShowImg !== this.imgList.length - 1) {
                this.ShowImg++
            } else {
                this.ShowImg = 0
            }
        },
    }
};
</script>
  
<style scoped>
/* 图片容器样式 */
.SwiperBox {
    position: relative;
    width: 500px;
    height: 300px;
    border: 1px solid #ccc;
    box-sizing: border-box;
    cursor: pointer;
}

/* 图片默认样式 */
.imgCss {
    position: absolute;
    left: 0px;
    top: 0px;
    width: 400px;
    height: 200px;
    opacity: 0;
    transition: 0.8s;
    /* 淡入淡出过渡时间 */
}

/* 图片选中样式(继承上方默认样式) */
.ShowCss {
    opacity: 1;
}

/* 两个按钮共有的样式,也可直接使用箭头图片替代 */
.leftBtn,
.rightBtn {
    position: absolute;
    top: 50%;
    transform: translateY(-50%);
    width: 30px;
    height: 30px;
    display: flex;
    justify-content: center;
    align-items: center;
    background: rgba(109, 109, 109, 0.445);
    color: #fff;
    border-radius: 50%;
    cursor: pointer;
    font-size: 12px;
    font-weight: 500;
}

.leftBtn {
    left: 10px;
}

.rightBtn {
    right: 10px;
}

/* 下方指示器盒子 */
.instBox {
    position: absolute;
    left: 50%;
    transform: translateX(-50%);
    bottom: 10px;
    display: flex;
}

/* 小圆点 */
.inst {
    width: 10px;
    height: 10px;
    border: 1px solid #ccc;
    margin-right: 8px;
    background: #fff;
    border-radius: 50%;
    cursor: pointer;
}

.inst:last-child {
    margin-right: 0px;
}

.instActv {
    border: 1px solid #ff0000;
    background: #ff0000;
}

#app {
    width: 100%;
    padding: 120px;
    display: flex;
    justify-content: center;
}
</style>
  
  