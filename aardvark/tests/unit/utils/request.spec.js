import request from '@/utils/request'
import Adaptor from 'axios-mock-adapter'

jest.mock('@/store', () => ({
  default: {
    state: {
      account: {
        token: 'eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJleHAiOiIyMDIxLTEwLTI0IDAwOjAwOjAwIiwibmFtZSI6InRvZ2V0dG95b3UifQ.XdF46NflSUjnt-adAc6rNZEXI1OD6nxtwGuhz9qkxUA',
      },
    },
  },
}))

describe('request', () => {
  let store
  let adaptor

  beforeEach(() => {
    store = require('@/store').default
    adaptor = new Adaptor(request, {delayResponse: 300})
  })

  afterEach(() => {
    adaptor.reset()
  })

  it('Show toast according to error code', (done) => {
    Promise.all(
      [
        [404, 'Resource not found'],
        [403, 'Operation forbidden'],
        [401, 'No permission for now'],
        [500, 'Server exception'],
      ].map(
        async ([code, message]) => {
          adaptor.onPost(new RegExp(code)).reply(() => [code])
          await expect(request.post(`/${code}`)).rejects.toThrow(`Request failed with status code ${code}`)
          const messageWrapper = global.bodyWrapper.findAll('.v-snack__content').at(-1)
          expect(messageWrapper.exists()).toBeTruthy()
          expect(messageWrapper.text()).toBe(message)
        }),
    ).then(() => done())
  })

  it('Add jwt in request header', async () => {
    adaptor.onPost(/test/).reply(() => [200])
    const {config} = await request.post('/test')
    expect(config.headers.Authorization).toEqual(`Bearer ${store.state.account.token}`)
  })
})
