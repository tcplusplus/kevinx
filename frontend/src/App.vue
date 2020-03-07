<template>
  <div id="app">
    <img alt="Vue logo" src="./assets/logo.png" />
    {{ data }}
    <vl-map :load-tiles-while-animating="true" :load-tiles-while-interacting="true"
             data-projection="EPSG:4326" style="height: 400px">
      <vl-view :zoom.sync="zoom" :center.sync="center" :rotation.sync="rotation"></vl-view>

      <vl-geoloc @update:position="geolocPosition = $event">
        <template slot-scope="geoloc">
          <vl-feature v-if="geoloc.position" id="position-feature">
            <vl-geom-point :coordinates="geoloc.position"></vl-geom-point>
            <vl-style-box>
              <vl-style-icon src="https://www.embregts-theunis.com/wp-content/uploads/2016/04/google-maps-marker-for-residencelamontagne-hi-188x300.png" :scale="0.4" :anchor="[0.5, 1]"></vl-style-icon>
            </vl-style-box>
          </vl-feature>
        </template>
      </vl-geoloc>

      <vl-geoloc @update:position="geolocPosition = $event">
        <template slot-scope="geoloc">
          <vl-feature v-if="geoloc.position" id="position-feature">
            <vl-geom-point :coordinates="geoloc.position"></vl-geom-point>
            <vl-style-box>
              <vl-style-icon src="https://www.embregts-theunis.com/wp-content/uploads/2016/04/google-maps-marker-for-residencelamontagne-hi-188x300.png" :scale="0.4" :anchor="[0.5, 1]"></vl-style-icon>
            </vl-style-box>
          </vl-feature>
        </template>
      </vl-geoloc>

      <vl-layer-tile id="osm">
        <vl-source-osm></vl-source-osm>
      </vl-layer-tile>
    </vl-map>
    <div style="padding: 20px">
      Zoom: {{ zoom }}<br>
      Center: {{ center }}<br>
      Rotation: {{ rotation }}<br>
      My geolocation: {{ geolocPosition }}
    </div>
  </div>
</template>

<script lang="ts">
import Vue from "vue";
import ReconnectingWebSocket from 'reconnecting-websocket';
import VueLayers from 'vuelayers'
import 'vuelayers/lib/style.css' // needs css-loader

Vue.use(VueLayers)

export default Vue.extend({
  name: "App",
  components: {
  },
  data: () => ({
    socket: new ReconnectingWebSocket('ws://localhost:5678/socket'),
    data: {},
    zoom: 2,
    center: [0, 0],
    rotation: 0,
    geolocPosition: undefined,
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
