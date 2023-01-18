import request from '@/utils/request'

/**
 * Added items
 * @param {Object} data
 * @return {Promise<any>}
 */
export const addProject = function (data = {}) {
  return request.post('/project', data)
}

/**
 * Edit item
 * @param {Object} data
 * @return {Promise<any>}
 */
export const editProject = function (data = {}) {
  return request.put('/project', data)
}

/**
 * delete an item
 * @param {Number | String} id
 * @return {Promise<any>}
 */
export const deleteProject = function (id) {
  return request.delete(`/project/${id}`)
}

/**
 * Query item details by id
 * @param {Number | String} id
 * @return {Promise<any>}
 */
export const getProject = function (id) {
  return request.get(`/project/${id}`)
}

/**
 * Query item list
 * @param {Object} query
 * @return {Promise<any>}
 */
export const getProjectList = function (query = {}) {
  return request.get('/project/list', {params: query})
}
