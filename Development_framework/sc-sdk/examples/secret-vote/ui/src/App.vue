<template>
  <div id="app">
    <router-view/>
  </div>
</template>

<style>
* {
  padding: 0;
  margin: 0;
}

#app {
  font-family: Avenir, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  background-image: url(assets/img/background.png);
  background-size: cover;
  width: 100%;
  height: auto;
  min-height: 100vh;
}

.top-nav {
  display: flex;
  width: 100%;
  height: 60px;
  line-height: 60px;
  background-color: #061829;
  color: #fff;
}

.top-nav .title {
  margin-left: 40px;
}

.top-nav .title a {
  padding-left: 8px;
  line-height: 20px;
  font-size: 14px;
  color: #0067ff;
}

.el-message {
  width: 400px;
  height: 40px;
  border-radius: 2px !important;
  padding: 10px 15px !important;
}

.el-message .el-message__content {
  font-size: 12px;
  line-height: 18px;
  color: #e8e8e8 !important;
}

.el-message.el-message--success {
  background-color: #162312;
  border: 1px solid #61d274;
}

.el-message.el-message--error {
  background-color: #2a1215;
  border: 1px solid #ed4b4b;
}

.el-message--success .el-message__closeBtn,
.el-message--success .el-message__closeBtn:hover {
  color: #61d274;
}

.el-message--error .el-message__closeBtn,
.el-message--error .el-message__closeBtn:hover {
  color: #ed4b4b;
}
</style>
