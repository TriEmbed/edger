<template>
  <FormDrawer
    v-model="visible"
    :loading="loading"
    :width="680"
    flat
    title="gpio Information"
  >
    <template #content>
      <v-form ref="form">
        <v-container>
          <v-row>
            <v-col cols="3">
              <v-text-field
                v-model="formData.gpioName"
                label="Pin                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             index"
                readonly="true"
              />
            </v-col>
            <v-col cols="3">
              <v-switch
                v-model="formData.model"
                :label="` ${formData.model?'input':'output'}`"
                flat
                hide-details
              />
            </v-col>
            <v-col cols="3">
              <v-text-field
                v-model="formData.MAC"
                label="MAC"
                readonly="true"
              />
            </v-col>
          </v-row>


          <v-row>
            <v-col cols="2">
              <v-text-field
                v-model="formData.CPUType"
                label="CPU"
                readonly="true"
              />
            </v-col>
            <v-col cols="2">
              <v-text-field
                v-model="formData.DeviceInstanceRevision"
                label="Revision"
                readonly="true"
              />
            </v-col>
            <v-col cols="3">
              <v-text-field
                v-model="formData.EMBFlash"
                label="EMB Flash"
                readonly="true"
              />
            </v-col>
            <v-col cols="3">
              <v-text-field
                v-model="formData.DEVICEINSTANCEFeatures"
                label="Features"
                readonly="true"
              />
            </v-col>
            <v-col cols="4">
              <v-text-field
                v-model="formData.MinimumFreeHeapSize"
                label="Minimum Free Heap Size"
                readonly="true"
              />
            </v-col>
            <v-col cols="4">
              <v-text-field
                v-model="formData.FreeHeapSize"
                label="Free Heap Size"
                readonly="true"
              />
            </v-col>
          </v-row>
          <v-row>
            <v-col cols="6">
              <v-text-field
                v-model="formData.Time"
                label="RTC current Time"
                readonly="true"
              />
            </v-col>
          </v-row>

          <v-row>
            <v-col cols="6">
              <v-text-field
                v-model="formData.ResetReason"
                label="Reset Reason"
                readonly="true"
              />
            </v-col>
          </v-row>
          <v-row>
            <v-col cols="3">
              <v-text-field
                v-model="formData.CompileDate"
                label="Compile Date"
                readonly="true"
              />
            </v-col>
            <v-col cols="2">
              <v-text-field
                v-model="formData.CompileTime"
                label="Compile Time"
                readonly="true"
              />
            </v-col>
            <v-col cols="6">
              <v-text-field
                v-model="formData.IDFVersion"
                label="IDF Version"
                readonly="true"
              />
            </v-col>
          </v-row>
          <v-row>
            <v-col cols="6">
              <v-text-field
                v-model.number="formData.Name"

                :rules="[
                  v => !!v || 'Please enter index',
                  v => v && v.length <= 32 || 'Enter up to 32 characters'
                ]"
                counter="32"
                label="index"
              />
            </v-col>
          </v-row>
          <v-row>
            <v-col
              class="py-0"
              cols="12"
            >
              <v-text-field
                v-model="formData.comment"
                :rules="[
                  v => !!v || 'Please enter Comments',
                  v => v && v.length <= 120 || 'Enter up to 120 characters'
                ]"
                counter="120"
                label="Comments"
              />
            </v-col>
          </v-row>
        </v-container>
      </v-form>
    </template>

    <template #actions>
      <v-spacer />
      <v-btn
        text
        x-large
        @click="close"
      >
        Cancel
      </v-btn>
      <v-btn
        text
        type="submit"
        x-large
        @click.stop.prevent="submit"
      >
        save
      </v-btn>
    </template>
  </FormDrawer>
</template>

<script>

import {addDevice, editDevice, getDevice} from '@/api/device'
import _ from 'lodash-es'

export default {
  mdnsBase: "esp-home",
  header: [
    {
      text: 'Number',
      align: 'center',
      sortable: false,
      value: 'number',
      width: 50,
      fixed: true,
    },
    {
      text: 'Pin index',
      align: 'center',
      sortable: false,
      value: 'gpioName',
    },
    {
      text: 'Analog',
      align: 'center',
      value: 'analog',
      width: 100,
    },
    {
      text: 'RTC',
      align: 'center',
      sortable: false,
      value: 'rtc',
      width: 120,
    },
    {
      text: 'CPU',
      align: 'center',
      value: 'CPUType',
      width: 120,
    },
    {
      text: 'Name',
      align: 'center',
      sortable: false,
      value: 'Name',
      width: 120,
    },
    {
      text: 'SPI Flash Chip MByte',
      align: 'center',
      value: 'SPIFlashChipMByte',
      width: 80,
    },
    {
      text: 'Compile Time',
      align: 'center',
      value: 'CompileTime',
      width: 100,
    },
    {
      text: 'Compile Date',
      align: 'center',
      value: 'CompileDate',
      width: 150,
    },
    {
      text: 'Operation',
      align: 'center',
      sortable: false,
      value: 'actions',
      width: 110,
      fixed: true,
    }],

  name: 'GpioSchema',
  props: {},
  data: () => ({
    formData: {
      model: true,
      id: '',
      analog: '',
      index: '',
      gpioName: '',
      DeviceInstanceRevision: '',
      DEVICEINSTANCEFeatures: '',
      MAC: '',
      Cores: '',
      SPIFlashChipMByte: "",
      EMBFlash: "",
      MinimumFreeHeapSize: "",
      Time: "",
      ResetReason: "",
      CompileDate: "",
      CompileTime: "",
      IDFVersion: "",
      FreeHeapSize: "",
      Name: "",
      Comments: "",
    },

    gpioPins: [
      {gpioName: 'GPIO0', index: 0, analog: "ADC2_CH1", rtc: 'RTC_GPIO11', comment: 'Strapping pin'},
      {gpioName: 'GPIO1', index: 1, comment: 'TXD'},
      {gpioName: 'GPIO2', index: 2, analog: 'ADC2_CH2', rtc: 'RTC_GPIO12', comment: 'Strapping pin'},
      {gpioName: 'GPIO3', index: 3, comment: 'RXD'},
      {gpioName: 'GPIO4', index: 4, analog: 'ADC2_CH0', rtc: 'RTC_GPIO10'},
      {gpioName: 'GPIO5', index: 5, comment: 'Strapping pin'},
      {gpioName: 'GPIO6', index: 6, comment: 'SPI0/1'},
      {gpioName: 'GPIO7', index: 7, comment: 'SPI0/1'},
      {gpioName: 'GPIO8', index: 8, comment: 'SPI0/1'},
      {gpioName: 'GPIO9', index: 9, comment: 'SPI0/1'},
      {gpioName: 'GPIO10', index: 10, comment: 'SPI0/1'},
      {gpioName: 'GPIO11', index: 11, comment: 'SPI0/1'},
      {gpioName: 'GPIO12', index: 12, analog: 'ADC2_CH5', rtc: 'RTC_GPIO15', comment: 'Strapping pin; JTAG'},
      {gpioName: 'GPIO13', index: 13, analog: 'ADC2_CH4', rtc: 'RTC_GPIO14', comment: 'JTAG'},
      {gpioName: 'GPIO14', index: 14, analog: 'ADC2_CH6', rtc: 'RTC_GPIO16', comment: 'JTAG'},
      {gpioName: 'GPIO15', index: 15, analog: 'ADC2_CH3', rtc: 'RTC_GPIO13', comment: 'Strapping pin; JTAG'},
      {gpioName: 'GPIO16', index: 16, comment: 'SPI0/1'},
      {gpioName: 'GPIO17', index: 17, comment: 'SPI0/1'},
      {gpioName: 'GPIO18', index: 18},
      {gpioName: 'GPIO19', index: 19},
      // missing pin
      {gpioName: 'GPIO21', index: 21},
      {gpioName: 'GPIO22', index: 22},
      {gpioName: 'GPIO23', index: 23},
      // missing pin
      {gpioName: 'GPIO25', index: 25, analog: 'ADC2_CH8', rtc: 'RTC_GPIO6'},
      {gpioName: 'GPIO26', index: 26, analog: 'ADC2_CH9', rtc: 'RTC_GPIO7'},
      {gpioName: 'GPIO27', index: 27, analog: 'ADC2_CH7', rtc: 'RTC_GPIO17'},
      // missing pins
      {gpioName: 'GPIO32', index: 32, analog: 'ADC1_CH4', rtc: 'RTC_GPIO9'},
      {gpioName: 'GPIO33', index: 33, analog: 'ADC1_CH5', rtc: 'RTC_GPIO8'},
      {gpioName: 'GPIO34', index: 34, analog: 'ADC1_CH6', rtc: 'RTC_GPIO4', comment: 'GPI'},
      {gpioName: 'GPIO35', index: 35, analog: 'ADC1_CH7', rtc: 'RTC_GPIO5', comment: 'GPI'},
      {gpioName: 'GPIO36', index: 36, analog: 'ADC1_CH0', rtc: 'RTC_GPIO0', comment: 'GPI'},
      {gpioName: 'GPIO37', index: 37, analog: 'ADC1_CH1', rtc: 'RTC_GPIO1', comment: 'GPI'},
      {gpioName: 'GPIO38', index: 38, analog: 'ADC1_CH2', rtc: 'RTC_GPIO2', comment: 'GPI'},
      {gpioName: 'GPIO39', index: 39, analog: 'ADC1_CH3', rtc: 'RTC_GPIO3', comment: 'GPI'},
    ],
    loading: false,
    visible: false,
  }),
  methods: {
    async getItem (id) {
      // this calls into the ap/device
      await getDevice(id).then((k) => {
        console.log(k.data)
      })
    },
    async add () {
      await addDevice(this.formData)
      this.$emit('addSuccess')
    },
    async edit () {
      await editDevice(this.formData)
      this.$emit('editSuccess')
    },
    async open (item) {
      try {
        this.visible = true
        this.loading = true
        if (item) {

          this.formData = _.pick(item, Object.keys(this.formData))
        }
      } finally {
        this.loading = false
      }
    },
    async close () {
      this.visible = false
      await this.$nextTick()
      Object.assign(this, this.$options.data.apply(this))
      this.$refs['form'].resetValidation()
      this.$refs['upload'].reset()
    },
    async submit () {
      if (!this.$refs['form'].validate()) return
      try {
        this.loading = true
        this.formData.id ? await this.edit() : await this.add()
        await this.close()
      } finally {
        this.loading = false
      }
    },
  },
}

export class gpioSchema {
}
</script>

<style lang="scss">
</style>
