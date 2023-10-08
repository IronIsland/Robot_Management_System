<template>
  <div class="app-container">
    <el-form inline> 
      <!-- 表单元素 -->
      <el-form-item>
        <el-input v-model="tempSearchObj.username" placeholder="Username" />
      </el-form-item>
      <!-- 查询与情况的按钮 -->
      <el-button type="primary" icon="el-icon-search" @click="search"
        >Search</el-button
      >
      <el-button type="default" @click="resetSearch">Clear</el-button>
    </el-form>
    <div style="margin-bottom: 20px">
      <!-- 添加与批量添加按钮 -->
      <el-button type="primary" @click="showAddUser">Add</el-button>
      <el-button
        type="danger"
        @click="revomveUsers"
        :disabled="selectedIds.length === 0"
        >Delete all</el-button
      >
    </div>
    <!-- table表格：展示用户信息的地方 -->
    <el-table
      border
      stripe
      v-loading="listLoading"
      :data="users"
      @selection-change="handleSelectionChange"
    >
      <el-table-column type="selection" width="55" />
      <el-table-column type="index" label="ID" width="80" align="center" />
      <el-table-column prop="username" label="Username" width="150" />
      <el-table-column prop="nickName" label="Nickname" />
      <el-table-column prop="roleName" label="roleName" />
      <el-table-column prop="gmtCreate" label="gmtCreate" width="180" />
      <el-table-column prop="gmtModified" label="gmtModified" width="180" />
      <el-table-column label="Operation" width="230" align="center">
        <template slot-scope="{ row }">
          <el-button
            type="info"
            size="mini"
            icon="el-icon-user-solid"
            title="Assign the Role"
            @click="showAssignRole(row)"
          ></el-button>
          <el-button
            type="primary"
            size="mini"
            icon="el-icon-edit"
            title="Modify the User info"
            @click="showUpdateUser(row)"
          ></el-button>
          <el-popconfirm
            :title="`Are you sure to delete ${row.username} ?`"
            @onConfirm="removeUser(row.id)"
          >
            <el-button
              style="margin-left: 10px"
              slot="reference"
              type="danger"
              size="mini"
              icon="el-icon-delete"
              title="Delete the user"
            ></el-button>
          </el-popconfirm>
        </template>
      </el-table-column>
    </el-table>
    <!-- 分页器 -->
    <el-pagination
      :current-page="page"
      :total="total"
      :page-size="limit"
      :page-sizes="[3, 10, 20, 30, 40, 50, 100]"
      style="padding: 20px 0"
      layout="prev, pager, next, jumper, ->, sizes, total"
      @current-change="getUsers"
      @size-change="handleSizeChange"
    />
    <!-- 对话框的结构 -->
    <el-dialog
      :title="user.id ? 'Modify User' : 'Add User'"
      :visible.sync="dialogUserVisible"
    >
      <el-form
        ref="userForm"
        :model="user"
        :rules="userRules"
        label-width="120px"
      >
        <el-form-item label="Username" prop="username">
          <el-input v-model="user.username" />
        </el-form-item>
        <el-form-item label="Nickname">
          <el-input v-model="user.nickName" />
        </el-form-item>

        <el-form-item v-if="!user.id" label="User Password" prop="password">
          <el-input v-model="user.password" />
        </el-form-item>
      </el-form>
      <div slot="footer" class="dialog-footer">
        <el-button @click="cancel">Cancel</el-button>
        <el-button :loading="loading" type="primary" @click="addOrUpdate"
          >Yes</el-button
        >
      </div>
    </el-dialog>

    <el-dialog
      title="Set the Role"
      :visible.sync="dialogRoleVisible"
      :before-close="resetRoleData"
    >
      <el-form label-width="80px">
        <el-form-item label="Username">
          <el-input disabled :value="user.username"></el-input>
        </el-form-item>

        <el-form-item label="Role list">
          <el-checkbox
            :indeterminate="isIndeterminate"
            v-model="checkAll"
            @change="handleCheckAllChange"
            >Choose all</el-checkbox
          >
          <div style="margin: 15px 0"></div>
          <el-checkbox-group
            v-model="userRoleIds"
            @change="handleCheckedChange"
          >
            <el-checkbox
              v-for="role in allRoles"
              :key="role.id"
              :label="role.id"
              >{{ role.roleName }}</el-checkbox
            >
          </el-checkbox-group>
        </el-form-item>
      </el-form>

      <div slot="footer">
        <el-button :loading="loading" type="primary" @click="assignRole"
          >Save</el-button
        >
        <el-button @click="resetRoleData">Delete</el-button>
      </div>
    </el-dialog>
  </div>
</template>

<script>
import cloneDeep from 'lodash/cloneDeep'

export default {
  name: 'AclUserList',

  data() {
    return {
      listLoading: false, // 是否显示列表加载的提示
      searchObj: {
        // 包含请求搜索条件数据的对象
        username: '',
      },
      tempSearchObj: {
        // 收集搜索条件输入的对象
        username: '',
      },
      selectedIds: [], // 所有选择的user的id的数组
      users: [], // 当前页的用户列表
      page: 1, // 当前页码
      limit: 3, // 每页数量
      total: 0, // 总数量
      user: {}, // 当前要操作的user
      dialogUserVisible: false, // 是否显示用户添加/修改的dialog
      userRules: {
        // 用户添加/修改表单的校验规则
        username: [
          { required: true, message: 'Please enter Username' },
          { min: 4, message: 'Username can not less than 4 character!' },
        ],
        password: [{ required: true, validator: this.validatePassword }],
      },
      loading: false, // 是否正在提交请求中
      dialogRoleVisible: false, // 是否显示角色Dialog
      allRoles: [], // 所有角色列表
      userRoleIds: [], // 用户的角色ID的列表
      isIndeterminate: false, // 是否是不确定的
      checkAll: false, // 是否全选
    }
  },

  //发请求一般情况下，我们都是在mounted去发，但是也可以在created内部去发
  created() {
    this.getUsers()
  },

  methods: {
    /* 
    显示指定角色的界面
    */
    showAssignRole(user) {
      this.user = user
      this.dialogRoleVisible = true
      this.getRoles()
    },

    /* 
    全选勾选状态发生改变的监听
    */
    handleCheckAllChange(value) {
      // value 当前勾选状态true/false
      // 如果当前全选, userRoleIds就是所有角色id的数组, 否则是空数组
      this.userRoleIds = value ? this.allRoles.map((item) => item.id) : []
      // 如果当前不是全选也不全不选时, 指定为false
      this.isIndeterminate = false
    },

    /* 
    异步获取用户的角色列表
    */
    async getRoles() {
      const result = await this.$API.user.getRoles(this.user.id)
      const { allRolesList, assignRoles } = result.data
      this.allRoles = allRolesList
      this.userRoleIds = assignRoles.map((item) => item.id)

      this.checkAll = allRolesList.length === assignRoles.length
      this.isIndeterminate =
        assignRoles.length > 0 && assignRoles.length < allRolesList.length
    },

    /* 
    角色列表选中项发生改变的监听
    */
    handleCheckedChange(value) {
      const { userRoleIds, allRoles } = this
      this.checkAll =
        userRoleIds.length === allRoles.length && allRoles.length > 0
      this.isIndeterminate =
        userRoleIds.length > 0 && userRoleIds.length < allRoles.length
    },

    /* 
    请求给用户进行角色授权
    */
    async assignRole() {
      const userId = this.user.id
      const roleIds = this.userRoleIds.join(',')
      this.loading = true
      const result = await this.$API.user.assignRoles(userId, roleIds)
      this.loading = false
      this.$message.success(result.message || 'Successful!')
      this.resetRoleData()

      // console.log(this.$store.getters.name, this.user)
      if (this.$store.getters.name === this.user.username) {
        window.location.reload()
      }
    },

    /* 
    重置用户角色的数据
    */
    resetRoleData() {
      this.dialogRoleVisible = false
      this.allRoles = []
      this.userRoleIds = []
      this.isIndeterminate = false
      this.checkAll = false
    },

    /* 
    自定义密码校验
    */
    validatePassword(rule, value, callback) {
      if (!value) {
        callback('Password can not be empty')
      } else if (!value || value.length < 6) {
        callback('Password can not less than 6 characters')
      } else {
        callback()
      }
    },
    /* 
    根据输入进行搜索
    */
    search() {
      this.searchObj = { ...this.tempSearchObj }
      this.getUsers()
    },

    /* 
    重置输入后搜索
    */
    resetSearch() {
      this.searchObj = {
        username: '',
      }
      this.tempSearchObj = {
        username: '',
      }
      this.getUsers()
    },

    /* 
    显示添加用户的界面
    */
    showAddUser() {
      this.user = {}
      this.dialogUserVisible = true

      this.$nextTick(() => this.$refs.userForm.clearValidate())
    },

    /* 
    删除所有选中的用户
    */
    revomveUsers() {
      this.$confirm('Sure to delete?')
        .then(async () => {
          await this.$API.user.removeUsers(this.selectedIds)
          this.$message.success('delete successfully!')
          this.getUsers()
        })
        .catch(() => {
          this.$message.info('Cancel')
        })
    },

    /* 
    列表选中状态发生改变的监听回调
    */
    handleSelectionChange(selection) {
      this.selectedIds = selection.map((item) => item.id)
    },

    /* 
    显示更新用户的界面
    */
    showUpdateUser(user) {
      this.user = cloneDeep(user)
      this.dialogUserVisible = true
    },

    /* 
    删除某个用户
    */
    async removeUser(id) {
      await this.$API.user.removeById(id)
      this.$message.success('Success to delete')
      this.getUsers(this.users.length === 1 ? this.page - 1 : this.page)
    },

    /* 
    获取分页列表
    */
    async getUsers(page = 1) {
      this.page = page
      const { limit, searchObj } = this
      this.listLoading = true
      const result = await this.$API.user.getPageList(page, limit, searchObj)
      this.listLoading = false
      const { items, total } = result.data
      this.users = items.filter((item) => item.username !== 'admin')
      this.total = total - 1
      this.selectedIds = []
    },

    /* 
    处理pageSize发生改变的监听回调
    */
    handleSizeChange(pageSize) {
      this.limit = pageSize
      this.getUsers()
    },

    /* 
    取消用户的保存或更新
    */
    cancel() {
      this.dialogUserVisible = false
      this.user = {}
    },

    /* 
    保存或者更新用户
    */
    addOrUpdate() {
      this.$refs.userForm.validate((valid) => {
        if (valid) {
          const { user } = this
          this.loading = true
          this.$API.user[user.id ? 'update' : 'add'](user)
            .then((result) => {
              this.loading = false
              this.$message.success('Saved!')
              this.getUsers(user.id ? this.page : 1)
              this.user = {}
              this.dialogUserVisible = false
            })
            .catch(() => {
              this.$message.error('Fail to save')
              this.loading = false
            })
        }
      })
    },
  },
}
</script>
