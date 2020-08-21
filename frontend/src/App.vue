<template>
  <div id="app">
    <img alt="Vue logo" src="./assets/kevinx.png" class="logo">
    <!-- <div>
      Recorders:
      <ul>
        <li v-for="(value, recorder) in data.recorders" :key="recorder">
          <label class="switch">
            <input type="checkbox" :checked="value" @change="changeRecorder(recorder)">
            <span class="slider round" /> {{ recorder }}
          </label>
        </li>
      </ul>
    </div>

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
    </table> -->
    <br>
    <b-card title="Sensors" class="card">
      <b-card-text>
        <Maps :location="location" class="map" />
        <table width="100%">
          <tr>
            <td width="15%" class="icontable">
              <v-icon name="map-marker" />
            </td>
            <td width="60%" class="datatable">
              {{ toGPS(data.gps_lat) }} N - &nbsp;&nbsp; {{ toGPS(data.gps_lon) }} E
            </td>
          </tr>
          <tr>
            <td class="icontable">
              <v-icon name="satellite" />
            </td>
            <td class="datatable">
              {{ Math.floor(data.gps_num_sat) }}
            </td>
          </tr>
          <tr>
            <td class="icontable">
              <v-icon name="expand-alt" />
            </td>
            <td class="datatable">
              {{ data.gps_alt }} (gps), {{ data.pressure_alt }} (pressure)
            </td>
          </tr>
          <tr>
            <td class="icontable">
              <v-icon name="clock" />
            </td>
            <td class="datatable">
              {{ data.time }}
            </td>
          </tr>
          <tr>
            <td class="icontable">
              <v-icon name="thermometer-half" />
            </td>
            <td class="datatable">
              {{ data.pressure_temp }}
            </td>
          </tr>
        </table>
      </b-card-text>
    </b-card>
  </div>
</template>

<script lang="ts">
import Vue from 'vue'
import ReconnectingWebSocket from 'reconnecting-websocket'
// @ts-ignore
import VueLayers from 'vuelayers'
import 'vuelayers/lib/style.css' // needs css-loader
import Maps from '@/components/Maps.vue'
import { BootstrapVue, IconsPlugin } from 'bootstrap-vue'

// Install BootstrapVue
Vue.use(BootstrapVue)
// Optionally install the BootstrapVue icon components plugin
Vue.use(IconsPlugin)

import 'bootstrap/dist/css/bootstrap.css'
import 'bootstrap-vue/dist/bootstrap-vue.css'

// or import all icons if you don't care about bundle size
import 'vue-awesome/icons'

/* Register component with one of 2 methods */

import Icon from 'vue-awesome/components/Icon.vue'

// globally (in your main .js file)
Vue.component('v-icon', Icon)

Vue.use(VueLayers)

export default Vue.extend({
  name: 'App',
  components: {
    Maps
  },
  data: () => ({
    socket: new ReconnectingWebSocket('ws://localhost:5678/socket'),
    data: {
      gps_lat: 0,
      gps_lon: 0
    },
    rocketposition: undefined
  }),
  computed: {
    location() {
      return [this.data.gps_lat, this.data.gps_lon]
    }
  },
  created() {
    this.socket.close()
    let ip = '' + window.location.href
    console.log(this.$route.query, window.location.href)
    ip = ip.replace('http://', '').replace('/', '')
    // ip = '141.135.128.158'
    ip = 'localhost:8080' // FIXME remove
    this.socket = new ReconnectingWebSocket('ws://' + ip + '/socket')
    this.socket.onmessage = this.newmessage
  },
  methods: {
    newmessage (message: MessageEvent) {
      this.data = JSON.parse(message.data)
    },
    changeRecorder (recorder: string) {
      // @ts-ignore
      this.socket.send(JSON.stringify({ action: 'recorder', recorder, value: !this.data.recorders[recorder] }))
    },
    toGPS (coord: number): string {
      const deg = Math.floor(coord)
      const rest = (coord - deg) * 60
      const min = Math.floor(rest)
      const sec = (rest - min) * 60
      const roundSec = Math.floor(sec * 1000) / 1000
      return `${deg} ${min}' ${roundSec}`
    }
  }
})
</script>

<style lang="scss">
#app {
    font-family: Avenir, Helvetica, Arial, sans-serif;
    -webkit-font-smoothing: antialiased;
    -moz-osx-font-smoothing: grayscale;
    text-align: center;
    color: #2c3e50;
    /* margin-top: 60px; */
    background-image: url('./assets/background.jpg');
    background-size: cover;
    width: 100%;
    height: 100%;
    position: absolute;
}

.logo {
  width: 50%;
  padding-bottom: 30px;
  filter: invert(100%);
  padding-top: 10px;
  position: relative;
}

.map {
  width: 80%;
  left: 10%;
  position: relative;
}

.card {
  width: 90%;
  left: 5%;
  .card-body {
    padding: 0.1rem;
  }
  .card-text {
    font: small;
  }
}

.icontable {
  text-align: right;
}

.datatable {
  text-align: left;
}

</style>
