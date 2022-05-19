<template>
  <div class="login-view">
    <div class="top-nav">
      <div class="title">
        匿名投票应用
        <a
          href="https://gitee.com/kunpengcompute/devkitdemo/tree/main/sc-sdk/examples/secret-vote"
          target="_blank"
          rel="noopener noreferrer"
          >查看说明文档</a
        >
      </div>
    </div>
    <div class="middle-container">
      <div class="welcome-part">
        <div class="title line-one">欢迎使用</div>
        <div class="title line-two">匿名投票应用</div>
        <div class="subtitle">基于鲲鹏TEE安全计算开发，</div>
        <div class="subtitle">在保证数据隐私安全的情况下完成匿名投票</div>
      </div>
      <el-form
        :model="loginInfo"
        class="login-form"
        label-position="top"
        :rules="rules"
        ref="validateLoginForm"
        @keyup.enter.native="handleLogin"
      >
        <el-alert
          v-if="showLoginErrorInfo"
          class="login-error-info"
          :title="loginErrorText"
          type="error"
          show-icon
        ></el-alert>
        <el-form-item label="用户名" prop="username">
          <el-input v-model="loginInfo.username" :clearable="true"></el-input>
          <div class="error-info" slot="error">
            <img src="../assets/img/icon-exclamation.svg" />
            {{ userInputError }}
          </div>
        </el-form-item>
        <el-form-item label="密码" prop="password">
          <el-input
            v-model="loginInfo.password"
            :clearable="true"
            :type="showPasswd ? 'text' : 'password'"
            oncut="return false"
            oncopy="return false"
          >
            <i class="eye-passwd" @click="changePasswdShow" slot="suffix">
              <img v-if="!showPasswd" src="../assets/img/icon-eye-close.svg" />
              <img v-if="showPasswd" src="../assets/img/icon-eye-open.svg" />
            </i>
          </el-input>
          <div class="error-info" slot="error">
            <img src="../assets/img/icon-exclamation.svg" />
            {{ pwdInputError }}
          </div>
        </el-form-item>
        <el-button class="login-button" @click="handleLogin">登录</el-button>
      </el-form>
    </div>
  </div>
</template>

<script>
import { setToken, setUserInfo } from "@/utils/auth";
import { doLogin, getVoteData } from "@/requestApi/request";

export default {
  name: "UserLogin",
  created() {
    this.getVoteInfo();
  },
  data() {
    let validateUserName = (rule, value, callback) => {
      if (value) {
        if (value.length >= 6 && value.length <= 32) {
          callback();
        } else {
          callback(new Error());
          this.userInputError = "用户名长度为6-32个字符";
        }
      } else {
        callback(new Error());
        this.userInputError = "用户名不能为空";
      }
    };
    let validatePwd = (rule, value, callback) => {
      if (value) {
        if (value.length >= 8 && value.length <= 32) {
          callback();
        } else {
          callback(new Error());
          this.pwdInputError = "密码长度为8-32个字符";
        }
      } else {
        callback(new Error());
        this.pwdInputError = "密码不能为空";
      }
    };
    return {
      showPasswd: false,
      userInputError: "",
      pwdInputError: "",
      showLoginErrorInfo: false,
      loginErrorText: "",
      loginInfo: {
        username: "",
        password: "",
      },
      rules: {
        username: [
          {
            validator: validateUserName,
            trigger: "blur",
          },
        ],
        password: [
          {
            validator: validatePwd,
            trigger: "blur",
          },
        ],
      },
    };
  },
  methods: {
    getVoteInfo() {
      getVoteData().then(() => {
        this.$router.push("/vote");
      });
    },
    changePasswdShow() {
      this.showPasswd = !this.showPasswd;
    },
    handleLogin() {
      this.$refs["validateLoginForm"].validate((valid) => {
        if (valid) {
          doLogin(this.loginInfo)
            .then((res) => {
              const cookie = document.cookie.substring(10);
              const { status, data } = res;
              if (status === 200) {
                setUserInfo(data);
                setToken(cookie);
                this.$router.push("/vote");
              }
            })
            .catch((error) => {
              const { data } = error.response;
              this.showLoginErrorInfo = true;
              this.loginErrorText = data.info_chinese || data.detail;
            });
        }
      });
    },
  },
};
</script>

<style scoped>
.middle-container {
  display: flex;
  margin: 0 auto;
  margin-top: 30vh;
  width: 70%;
  justify-content: space-between;
}

.welcome-part .title {
  line-height: 108px;
  font-size: 72px;
  color: #fff;
}

.subtitle {
  line-height: 30px;
  font-size: 20px;
  color: #cacaca;
}

.login-form {
  position: relative;
}

.login-error-info {
  position: absolute;
  left: 0;
  transform: translateY(calc(-100% - 12px));
  width: 400px;
  border-radius: 2px;
  background-color: #2a1215;
  border: 1px solid #ed4b4b;
}

.login-error-info.el-alert--error.is-light {
  background-color: #2a1215;
}

.login-error-info.el-alert--error.is-light >>> .el-alert__title {
  font-size: 12px;
  color: #e8e8e8;
}

.login-error-info.el-alert--error.is-light >>> .el-alert__closebtn {
  font-size: 16px;
  color: #ed4b4b;
  top: 10px;
}

.login-form >>> .el-form-item {
  margin-bottom: 32px;
}

.login-form >>> .el-form-item__label {
  line-height: 20px;
  color: #fff;
}

.login-form >>> .el-input__inner {
  width: 400px;
  height: 38px;
  padding: 0;
  padding-bottom: 10px;
  line-height: 28px;
  border: none;
  border-bottom: 1px solid #d8d8d8;
  border-radius: 0;
  font-size: 18px;
  color: #fff;
  background-color: transparent;
}

.login-form >>> .el-input__inner:-webkit-autofill,
.login-form >>> .el-input__inner:-webkit-autofill:hover,
.login-form >>> .el-input__inner:-webkit-autofill:focus,
.login-form >>> .el-input__inner:-webkit-autofill:active {
  -webkit-transition-delay: 99999s;
  -webkit-transition: color 99999s ease-out, background-color 99999s ease-out;
}

.login-form .eye-passwd {
  display: inline-flex;
  vertical-align: middle;
  margin-bottom: 4px;
  cursor: pointer;
}

.login-form >>> .el-input__inner:focus {
  border-bottom-color: #0067ff;
}

.login-button {
  box-sizing: border-box;
  margin-top: 8px;
  width: 400px;
  height: 48px;
  font-size: 20px;
  background-color: transparent;
  border: 1px solid #0067ff;
  color: #0067ff;
  border-radius: 1px;
}

.login-button:focus,
.login-button:hover {
  background-color: transparent;
  border: 1px solid #0067ff;
  color: #0067ff;
}

.login-form >>> .el-form-item.is-error .el-input__inner {
  border-color: #ed4b4b;
}

.login-form >>> .error-info,
.login-form .el-form-item.is_error .error-info {
  position: absolute;
  display: flex;
  left: 0;
  margin-top: 5px;
  line-height: 16px;
  color: #ed4b4b;
}

.login-form >>> .error-info img,
.login-form .el-form-item.is-error .error-info img {
  margin-right: 5px;
  width: 16px;
  height: 16px;
}
</style>