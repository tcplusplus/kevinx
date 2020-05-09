<template>
  <div id="app">
    <img alt="Vue logo" src="./assets/kevinx.png">
    {{ data }}
    <table>
      <tr>
        <th>Sensor</th>
        <th>X</th>
        <th>Y</th>
        <th>Z</th>
      </tr>
      <tr>
        <td>acc</td>
        <td>{{ data.acc_x }}</td>
        <td>{{ data.acc_y }}</td>
        <td>{{ data.acc_z }}</td>
      </tr>
      <tr>
        <td>giro</td>
        <td>{{ data.giro_x }}</td>
        <td>{{ data.giro_y }}</td>
        <td>{{ data.giro_z }}</td>
      </tr>
    </table>

    GPS: lat={{ data.gps_lat }} lon={{ data.gps_lon }} ( {{ data.gps_num_sat }} sat)<br>
    Altitude: {{ data.gps_alt }} (from gps), {{ data.pressure_alt }} (from pressure)<br>
    Temp:  {{ data.pressure_temp }}<br>
    Time: {{ data.time }}
    <Maps :location="location" />
  </div>
</template>

<script lang="ts">
import Vue from 'vue'
import ReconnectingWebSocket from 'reconnecting-websocket'
// @ts-ignore
import VueLayers from 'vuelayers'
import 'vuelayers/lib/style.css' // needs css-loader
import Maps from '@/components/Maps.vue'

Vue.use(VueLayers)

export default Vue.extend({
  name: 'App',
  components: {
    Maps
  },
  data: () => ({
    socket: new ReconnectingWebSocket('ws://localhost:5678/socket'),
    data: {},
    rocketposition: undefined
  }),
  computed: {
    location() {
      return [0, 0]
    }
  },
  created() {
    this.socket.close()
    let ip = '' + window.location.href
    console.log(this.$route.query, window.location.href)
    ip = ip.replace('http://', '').replace('/', '')
    // ip = '141.135.128.158'
    this.socket = new ReconnectingWebSocket('ws://' + ip + '/socket')
    this.socket.onmessage = this.newmessage
  },
  methods: {
    newmessage(message: MessageEvent) {
      this.data = JSON.parse(message.data)
    }
  }
})
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
