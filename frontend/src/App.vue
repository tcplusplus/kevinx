<template>
  <div id="app">
    <img alt="Vue logo" src="./assets/logo.png" />
    {{ data }}
    <Maps :location="location" />

  </div>
</template>

<script lang="ts">
import Vue from "vue";
import ReconnectingWebSocket from 'reconnecting-websocket';
import VueLayers from 'vuelayers';
import 'vuelayers/lib/style.css' // needs css-loader
import Maps from '@/components/Maps.vue'

Vue.use(VueLayers)

export default Vue.extend({
  name: "App",
  components: {
    Maps
  },
  data: () => ({
    socket: new ReconnectingWebSocket('ws://localhost:5678/socket'),
    data: {},
    rocketposition: undefined
  }),
  created () {
    this.socket.onmessage = this.newmessage
  },
  computed: {
    location () {
      return [0, 0]
    }
  },
  methods: {
    newmessage (message: MessageEvent) {
      this.data = JSON.parse(message.data)
    } 
  }
});
</script>

<style>
#app {
  font-family: Avenir, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
  margin-top: 60px;
}
</style>
