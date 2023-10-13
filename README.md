# vue-admin-template

Some notice before start:

1. Our backend database is using a shared link: http://gmall-h5-api.atguigu.cn, and due to its large visitor volume, we found that some function of our website (Attribute and Appearance of our website) are sometimes inaccessible (
Request failed with status code 503). In that case, please wait for the website to response, or just skip this function.

2. Steps to change the dashboard data:

   (1) Using raw_data/tojson.py and modify the  txt path of the line 3 and json name in line 40,for example:

   fp = open("output.txt", 'r')
   
   with open('1.json',mode="w+",encoding='utf-8') as f:

   That means convert output.txt into 1.json.

   (2) copy the data in json, eg:

   "x_dis": [50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50], "y_dis": [30, 30, 30, 25, 25, 25, 25, 25, 25, 25, 25, 25, 26, 26, 26, 26, 26, 26, 25, 25, 25, 35, 35, 35, 46, 46, 46, 42, 42, 42, 40, 40, 40], "z_dis": [11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11], "dis_axis": ["1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31", "32", "33"]

   and replace the same attrbute in the ./mock/data.json (line 66).

   (3) Restart the server and check the result.

   (4) Due to the echarts multiple curve(see exsample at https://echarts.apache.org/examples/zh/editor.html?c=line-stack), the actual value of the curve is not equal to the curve it shows. In that case, please click the legend above the curve to enable/disable the curve, and it becomes accurate when only one curve is shown. 

   (5) Change the image: please modify the path in ./src/views/dashboard/Observe/Rolling/index.vue to show the image you like.


The following is the origin README.md, you can config this program as follows:

English | [简体中文](./README-zh.md)

> A minimal vue admin template with Element UI & axios & iconfont & permission control & lint

Personal learning and practice program
The following is the background frame address of panjiachen

[Online address](http://panjiachen.github.io/vue-admin-template)
[Home visit](https://panjiachen.gitee.io/vue-admin-template)

**Live demo:** http://panjiachen.github.io/vue-admin-template

**The current version is `v4.0+` build on `vue-cli`. If you want to use the old version , you can switch branch to [tag/3.11.0](https://github.com/PanJiaChen/vue-admin-template/tree/tag/3.11.0), it does not rely on `vue-cli`**

<p align="center">
  <b>SPONSORED BY</b>
</p>
<p align="center">
   <a href="https://finclip.com?from=vue_element" title="FinClip" target="_blank">
      <img height="200px" src="https://gitee.com/panjiachen/gitee-cdn/raw/master/vue%E8%B5%9E%E5%8A%A9.png" title="FinClip">
   </a>
</p>




## Build Setup

```bash
# (for Windows)install nvm
https://github.com/coreybutler/nvm-windows/releases
#Then choose the nvm-steup.exe to install nvm

#check if nvm available
nvm -v

#Install node.js and npm using the nvm
nvm install 12.17.0

#Use the nodejs
nvm use 12.17.0 

#check if nvm available
npm -v

# clone the project
git clone https://github.com/IronIsland/Robot_Management_System
# panjiacheng template（we modified from it）
#git clone https://github.com/PanJiaChen/vue-admin-template.git

# enter the project directory
cd Robot_Management_System

# install dependency
npm install

# develop
npm run dev
```

This will automatically open http://localhost:9528

## Build

```bash
# build for test environment
npm run build:stage

# build for production environment
npm run build:prod
```

## Advanced

```bash
# preview the release environment effect
npm run preview

# preview the release environment effect + static resource analysis
npm run preview -- --report

# code format check
npm run lint

# code format check and auto fix
npm run lint -- --fix
```

Refer to [Documentation](https://panjiachen.github.io/vue-element-admin-site/guide/essentials/deploy.html) for more information

## Demo

![demo](src/assets/test.png)

## Extra

If you want router permission && generate menu by user roles , you can use this branch [permission-control](https://github.com/PanJiaChen/vue-admin-template/tree/permission-control)

For `typescript` version, you can use [vue-typescript-admin-template](https://github.com/Armour/vue-typescript-admin-template) (Credits: [@Armour](https://github.com/Armour))

## Related Project

- [vue-element-admin](https://github.com/PanJiaChen/vue-element-admin)

- [electron-vue-admin](https://github.com/PanJiaChen/electron-vue-admin)

- [vue-typescript-admin-template](https://github.com/Armour/vue-typescript-admin-template)

- [awesome-project](https://github.com/PanJiaChen/vue-element-admin/issues/2312)

## Browsers support

Modern browsers and Internet Explorer 10+.

| [<img src="https://raw.githubusercontent.com/alrra/browser-logos/master/src/edge/edge_48x48.png" alt="IE / Edge" width="24px" height="24px" />](http://godban.github.io/browsers-support-badges/)</br>IE / Edge | [<img src="https://raw.githubusercontent.com/alrra/browser-logos/master/src/firefox/firefox_48x48.png" alt="Firefox" width="24px" height="24px" />](http://godban.github.io/browsers-support-badges/)</br>Firefox | [<img src="https://raw.githubusercontent.com/alrra/browser-logos/master/src/chrome/chrome_48x48.png" alt="Chrome" width="24px" height="24px" />](http://godban.github.io/browsers-support-badges/)</br>Chrome | [<img src="https://raw.githubusercontent.com/alrra/browser-logos/master/src/safari/safari_48x48.png" alt="Safari" width="24px" height="24px" />](http://godban.github.io/browsers-support-badges/)</br>Safari |
| --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| IE10, IE11, Edge                                                                                                                                                                                                | last 2 versions                                                                                                                                                                                                   | last 2 versions                                                                                                                                                                                               | last 2 versions                                                                                                                                                                                               |

## License

[MIT](https://github.com/PanJiaChen/vue-admin-template/blob/master/LICENSE) license.

Copyright (c) 2017-present PanJiaChen
