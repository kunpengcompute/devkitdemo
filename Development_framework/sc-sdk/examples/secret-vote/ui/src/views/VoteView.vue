<template>
  <div class="vote-view">
    <div class="top-nav">
      <div class="title">
        匿名投票应用
        <a
          href="https://gitee.com/kunpengcompute/devkitdemo/tree/main/Development_framework/sc-sdk/examples/secret-vote"
          target="_blank"
          rel="noopener noreferrer"
          >查看说明文档</a
        >
      </div>
      <div class="nav-right-part">
        <div class="show-username">{{ userInfo.username }}</div>
        <div class="sep"></div>
        <div class="logout" title="退出登录" @click="handleLogout()"></div>
      </div>
    </div>
    <div class="vote-info">
      <div class="vote-title">{{ voteTitle }}</div>
      <div class="vote-detail">{{ voteDetail }}</div>
    </div>
    <div class="color-sep">
      <el-alert
        v-if="showVoteErrorInfo"
        class="vote-error-info"
        :title="voteErrorText"
        type="error"
        show-icon
        @close="closeVoteError"
      ></el-alert>
    </div>

    <el-radio-group
      v-if="!voteStatus"
      v-model="voteSelectId"
      class="vote-radio-group"
    >
      <el-radio
        v-for="item in voteOption"
        :key="item.vote_id"
        :label="item.vote_id"
        border
        >{{ item.vote_opt_message }}</el-radio
      >
    </el-radio-group>

    <ul class="show-vote-result" v-if="voteStatus">
      <li class="result-item" v-for="item in voteOption" :key="item.option_id">
        <div
          class="mask"
          :style="{ width: percentWidth(item.votes_number) }"
        ></div>
        <div class="vote-message">{{ item.vote_opt_message }}</div>
        <div class="right-part">
          {{
            `${item.votes_number}(${
              Math.round((item.votes_number / voteCount) * 10000) / 100
            }%)`
          }}
        </div>
      </li>
    </ul>

    <el-button
      v-if="!voteStatus"
      class="vote-button"
      :disabled="buttonDisabled"
      :class="voteSelectId !== '' ? '' : 'disable-button'"
      @click="handleVote()"
      :title="voteSelectId !== '' ? '' : '请选择一个选项'"
      >投票</el-button
    >

    <div v-if="voteStatus" class="vote-sum">
      您已投票，现有{{ voteCount }}人参与投票
    </div>
  </div>
</template>

<script>
import { getUserInfo, removeToken, removeUserInfo } from "@/utils/auth";
import { doLogout, doVote, getVoteData } from "@/requestApi/request";
import { Message } from "element-ui";

export default {
  name: "VoteView",
  data() {
    return {
      voteStatus: false,
      voteSelectId: "",
      voteTitle: "",
      voteDetail: "",
      showVoteErrorInfo: false,
      voteErrorText: "",
      voteOption: [],
      buttonDisabled: false,
    };
  },
  created() {
    this.getVoteInfo();
  },
  computed: {
    userInfo() {
      return getUserInfo() || {};
    },
    voteCount() {
      let count = 0;
      this.voteOption.forEach((item) => {
        count += item.votes_number;
      });
      return count;
    },
  },
  methods: {
    closeVoteError() {
      this.showVoteErrorInfo = false;
    },
    getVoteInfo() {
      getVoteData().then((res) => {
        const { data } = res.data;
        this.voteStatus = data.vote_status;
        this.voteOption = data.vote_info_list;
        this.voteTitle = data.vote_title;
        this.voteDetail = data.vote_sub_title;
      });
    },
    handleLogout() {
      doLogout().then((res) => {
        const { status, data } = res;
        if (status === 200) {
          removeToken();
          removeUserInfo();
          this.$router.push("/login");
          Message({
            type: "success",
            showClose: true,
            duration: 3 * 1000,
            message: data.info_chinese,
            offset: 84,
          });
        }
      });
    },
    handleVote() {
      if (this.voteSelectId !== "") {
        const voteInfo = {
          id: this.userInfo.id,
          vote_id: this.voteSelectId,
        };
        this.buttonDisabled = true;
        doVote(voteInfo)
          .then(() => {
            this.getVoteInfo();
            this.buttonDisabled = false;
          })
          .catch((error) => {
            if (error.response) {
              const { status } = error.response;
              if (status === 403) {
                const { data } = error.response;
                this.showVoteErrorInfo = true;
                this.voteErrorText = data.info_chinese;
                this.getVoteInfo();
                this.buttonDisabled = false;
              }
            }
          });
      }
    },
    percentWidth(itemCount) {
      const ITEMWIDTH = 596;
      const percentW =
        (Math.round((itemCount / this.voteCount) * 10000) / 10000) * ITEMWIDTH;
      return percentW + "px";
    },
  },
};
</script>

<style scoped>
.vote-view {
  overflow: hidden;
}

.top-nav {
  display: flex;
  justify-content: space-between;
}

.nav-right-part {
  display: flex;
}

.nav-right-part .sep {
  margin: 24px 20px;
  width: 1px;
  height: 12px;
  opacity: 0.5;
  background-color: #fff;
}

.nav-right-part .logout {
  margin-top: 22px;
  margin-right: 20px;
  width: 16px;
  height: 16px;
  background-image: url(../assets/img/icon-logout.svg);
  background-size: contain;
  transform: rotate(90deg);
  cursor: pointer;
}

.vote-view .vote-info {
  margin: 90px auto 0;
  width: 1080px;
  border-radius: 2px;
  background: rgba(6, 11, 21, 0.65);
  background-image: radial-gradient(
    circle at 50% 100%,
    rgba(10, 137, 255, 0.21) 0%,
    rgba(0, 0, 0, 0) 39%
  );
  text-align: center;
  overflow: hidden;
}

.vote-info .vote-title {
  margin: 40px auto 20px;
  line-height: 30px;
  font-size: 20px;
  color: #e8e8e8;
}

.vote-info .vote-detail {
  margin: 0 auto 25px;
  line-height: 20px;
  font-size: 16px;
  color: #cacaca;
}

.vote-view .color-sep {
  position: relative;
  margin: 0 auto 75px;
  width: 1080px;
  height: 4px;
  background-image: linear-gradient(
    90deg,
    #43abf6 0%,
    #2072ea 39%,
    #3fcdf5 85%,
    #2072ea 100%
  );
  border-radius: 0 0 2px 2px;
}

.vote-error-info {
  position: absolute;
  left: 0;
  transform: translate(340px, 25px);
  width: 400px;
  border-radius: 2px;
  border: 1px solid #ed4b4b;
}

.vote-error-info.el-alert--error.is-light {
  background-color: #2a1215;
}

.vote-error-info.el-alert--error.is-light >>> .el-alert__title {
  font-size: 12px;
  color: #e8e8e8;
}

.vote-error-info.el-alert--error.is-light >>> .el-alert__closebtn {
  font-size: 16px;
  top: 10px;
  color: #ed4b4b;
}

.vote-radio-group {
  display: block;
  margin: 0 auto;
  width: 600px;
  overflow: hidden;
}

.vote-radio-group .el-radio {
  display: block;
  box-sizing: border-box;
  width: 600px;
  height: 54px;
  background: rgba(6, 11, 21, 0.65);
  background-image: radial-gradient(
    circle at 50% 100%,
    rgba(10, 137, 255, 0.21) 0%,
    rgba(0, 0, 0, 0) 39%
  );
  border: 2px solid rgba(255, 255, 255, 0.4);
  border-radius: 2px;
  box-shadow: 0 8px 16px 0 rgba(77, 77, 77, 0.16);
}

.vote-radio-group .el-radio.is-bordered {
  padding: 10px 20px;
  margin-bottom: 40px;
  line-height: 30px;
}

.vote-radio-group .el-radio.is-bordered.is-checked {
  border: 2px solid #2e6aff;
  box-shadow: 0 8px 16px 0 rgba(46, 106, 255, 0.3);
}

.vote-radio-group .el-radio.is-bordered + .el-radio.is-bordered {
  margin-left: 0px;
}

.vote-radio-group >>> .el-radio__input {
  line-height: 24px;
}

.vote-radio-group >>> .el-radio__inner {
  width: 24px;
  height: 24px;
  background-color: transparent;
  border: 2px solid #e0e0e0;
}

.vote-radio-group >>> .el-radio__input.is-checked .el-radio__inner {
  border: 2px solid #267dff;
  background-color: transparent;
}

.vote-radio-group >>> .el-radio__label {
  font-size: 20px;
  color: #e8e8e8;
}

.vote-radio-group >>> .el-radio__input.is-checked + .el-radio__label {
  color: #e8e8e8;
}

.vote-radio-group >>> .el-radio__inner::after {
  width: 16px;
  height: 16px;
  background-color: #267dff;
}

.vote-view .vote-button {
  display: block;
  margin: 0 auto;
  padding: 0;
  width: 160px;
  height: 32px;
  line-height: 18px;
  font-size: 18px;
  border-radius: 2px;
  border: 1px solid #267dff;
  background-color: rgba(6, 11, 21, 0.85);
  color: #267dff;
}

.vote-view .vote-button.disable-button {
  border: 1px solid #979797;
  color: #aaa;
  cursor: default;
}

.vote-button.el-button.is-disabled,
.vote-button.el-button.is-disabled:focus,
.vote-button.el-button.is-disabled:hover {
  color: #aaa;
  background-color: rgba(6, 11, 21, 0.85);
  border: 1px solid #979797;
  cursor: default;
}

.show-vote-result {
  margin: 0 auto;
  width: 600px;
  overflow: hidden;
}

.show-vote-result .result-item {
  position: relative;
  box-sizing: border-box;
  display: flex;
  justify-content: space-between;
  width: 600px;
  height: 54px;
  margin-bottom: 40px;
  padding: 10px 20px;
  line-height: 30px;
  font-size: 20px;
  color: #e8e8e8;
  background-color: rgba(6, 11, 21, 0.65);
  background-image: radial-gradient(
    circle at 50% 100%,
    rgba(10, 137, 255, 0.21) 0%,
    rgba(0, 0, 0, 0) 39%
  );
  border: 2px solid rgba(255, 255, 255, 0.4);
  border-radius: 2px;
  box-shadow: 0 8px 16px 0 rgba(77, 77, 77, 0.16);
}

.show-vote-result .result-item .mask {
  position: absolute;
  top: 0;
  left: 0;
  z-index: 1;
  height: 50px;
  border-radius: 2px;
  background-color: rgba(34, 111, 221, 0.65);
}

.show-vote-result .result-item .vote-message,
.show-vote-result .result-item .right-part {
  z-index: 10;
}

.vote-sum {
  margin: 0 auto;
  width: fit-content;
  font-size: 18px;
  color: #fff;
}
</style>