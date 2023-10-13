const getters = {
  sidebar: state => state.app.sidebar,
  device: state => state.app.device,
  token: state => state.user.token,
  avatar: state => state.user.avatar,
  name: state => state.user.name,
  //角色
  role(state) {
    const arr = []
    if (state.genshin.record.list) {
      state.genshin.record.list.forEach(element => {
        if (element.gacha_type == 301 || element.gacha_type == 400) {
          arr.push(element)
        }
      });
    }
    return arr
  },
  //武器
  weapons(state) {
    const arr = []
    if (state.genshin.record.list) {
      state.genshin.record.list.forEach(element => {
        if (element.gacha_type == 302) {
          arr.push(element)
        }
      });
    }
    return arr
  },
  //常驻
  resident(state) {
    const arr = []
    if (state.genshin.record.list) {
      state.genshin.record.list.forEach(element => {
        if (element.gacha_type == 200) {
          arr.push(element)
        }
      });
    }
    return arr
  }
}
export default getters
