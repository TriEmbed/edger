import axios from 'axios'
import qs from 'qs'
import toast from '@/utils/toast'
// nje: this is as it fake internal endpoint in real life this would be an authiticator
const request = axios.create({
  baseURL: '/api',
  responseType: 'json',
  validateStatus: status => status === 200,
  paramsSerializer (params) {
    // https://github.com/axios/axios/issues/1644
    return qs.stringify(params)
  },
})

request.interceptors.request.use(
  config => {
    const store = require('@/store').default
    const {token} = store.state.account
    if (token) {
      config.headers['Authorization'] = `Bearer ${token}`
    }
    return config
  },
)

request.interceptors.response.use(
  response => response,
  error => {
    let message = ''
    if (error.response) {
      switch (error.response.status) {
        case 404:
          message = 'Resource not found'
          break
        case 403:
          message = 'Operation forbidden'
          break
        case 401:
          message = 'No permission for now'
          break
        case 500:
          message = 'Server exception'
          break
        default:
          break
      }
    } else {
      message = error.message === 'Network Error' ? 'Network Anomaly' : error.message
    }
    toast.error({message})
    return Promise.reject(error)
  },
)

export default request
