import request from '@/utils/request'

/**
 * Added items
 * @param {Object} data
 * @return {Promise<any>}
 */
export const addDevice = function (data = {}) {
  return request.post('/device', data)
}

/**
 * Edit item
 * @param {Object} data
 * @return {Promise<any>}
 */
export const editDevice = function (data = {}) {

  return request.put('/device', data)
}

/**
 * delete an item
 * @param {Number | String} id
 * @return {Promise<any>}
 */
export const deleteDevice = function (id) {
  return request.delete(`/device/${id}`)
}

/**
 * Query item details by id
 * @param {Number | String} id
 * @return {Promise<any>}
 */
export const getDevice = function (id) {
  return request.get(`/device/item/${id}`)
}

/**
 * Query item list
 * @param {Object} query
 * @return {Promise<any>}
 */
export const getDeviceList = function (query = {}) {
  return request.get('/device/list', {params: query})
}
