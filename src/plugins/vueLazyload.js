import Vue from 'vue'

import VueLazyload from 'vue-lazyload'
import error from '@/assets/lazyload/error.png'
import loading from '@/assets/lazyload/loading.gif'
Vue.use(VueLazyload, {
    error: error,
    loading: loading,
})