const data = require('./100083870.json')

module.exports = [{
    url: '/genshin/list',
    type: 'get',
    response: config => {
        return {
            code: 20000,
            data
        }
    }
}]