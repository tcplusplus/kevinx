<template>
  <div id="app">
    <img alt="Vue logo" src="./assets/kevinx.png" class="logo">
    <b-card title="Status" class="card">
      <b-card-text>
        <table width="100%">
          <tr>
            <td width="15%">
              <v-icon name="wifi" />
            </td>
            <td width="70%" class="datatable">
              Connection
            </td>
            <td width="15%">
              <v-icon name="star" :style="{color: connected ? 'green' : 'red'}" />
            </td>
          </tr>
          <tr>
            <td width="15%">
              <v-icon name="inbox" />
            </td>
            <td width="70%" class="datatable">
              Data
            </td>
            <td width="15%">
              <v-icon name="star" :style="{color: receiving ? 'green' : 'red'}" />
            </td>
          </tr>
          <tr>
            <td width="15%">
              <v-icon name="video" />
            </td>
            <td width="70%" class="datatable">
              Record
            </td>
            <td width="15%">
              <b-form-checkbox
                v-model="data.recorders.CSV"
                name="check-button"
                switch
                @change="changeRecorder('CSV', !data.recorders.CSV)"
              />
            </td>
          </tr>
          <tr>
            <td width="15%">
              <v-icon name="download" />
            </td>
            <td width="70%" class="datatable">
              <a href="/recordings" target="blank">Downloads</a>
            </td>
            <td width="15%" />
          </tr>
        </table>
      </b-card-text>
    </b-card>
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
        <table width="100%">
          <tr>
            <th class="icontable" width="15%">
              <v-icon name="tachometer-alt" />
            </th>
            <th width="20%">
              X
            </th>
            <th width="20%">
              Y
            </th>
            <th width="20%">
              Z
            </th>
          </tr>
          <tr>
            <td class="icontable">
              acc
            </td>
            <td>{{ data.acc_x }}</td>
            <td>{{ data.acc_y }}</td>
            <td>{{ data.acc_z }}</td>
          </tr>
          <tr>
            <td class="icontable">
              giro
            </td>
            <td>{{ data.giro_x }}</td>
            <td>{{ data.giro_y }}</td>
            <td>{{ data.giro_z }}</td>
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
import 'bootstrap/dist/css/bootstrap.css'
import 'bootstrap-vue/dist/bootstrap-vue.css'
import 'vue-awesome/icons'
import Icon from 'vue-awesome/components/Icon.vue'

Vue.component('v-icon', Icon)
Vue.use(VueLayers)
Vue.use(BootstrapVue)
Vue.use(IconsPlugin)

export default Vue.extend({
  name: 'App',
  components: {
    Maps
  },
  data: () => ({
    socket: new ReconnectingWebSocket('ws://localhost:5678/socket'),
    data: {
      gps_lat: 0,
      gps_lon: 0,
      gps_num_sat: 0,
      pressure_temp: 0,
      acc_x: 0,
      acc_y: 0,
      acc_z: 0,
      giro_x: 0,
      giro_y: 0,
      giro_z: 0,
      recorders: {
        CSV: false
      }
    },
    connected: false,
    lastReceived: 0,
    now: (new Date()).getTime()
  }),
  computed: {
    location(): number[] {
      return [this.data.gps_lat, this.data.gps_lon]
    },
    receiving(): boolean {
      return this.now - 20000 < this.lastReceived
    }
  },
  created() {
    this.socket.close()
    let ip = '' + window.location.href
    console.log(this.$route.query, window.location.href)
    ip = ip.replace('http://', '').replace('/', '')
    // ip = '141.135.128.158'
    // ip = 'localhost:8080' // FIXME remove
    this.socket = new ReconnectingWebSocket('ws://' + ip + '/socket')
    this.socket.onmessage = this.newmessage
    this.socket.onopen = () => this.connected = true
    this.socket.onclose = () => this.connected = false
    setInterval(() => this.now = (new Date()).getTime(), 1000)
  },
  methods: {
    newmessage (message: MessageEvent) {
      this.data = JSON.parse(message.data)
      this.lastReceived = (new Date()).getTime()
    },
    changeRecorder (recorder: string, value: boolean) {
      // @ts-ignore
      this.socket.send(JSON.stringify({ action: 'recorder', recorder, value }))
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
  padding-left: 10px;
}

</style>
