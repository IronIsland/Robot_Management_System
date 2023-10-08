<template>
  <div>
    <el-form inline>
      <el-form-item>
        <el-input v-model="tempSearchObj.roleName" placeholder="Role Name" />
      </el-form-item>

      <el-button type="primary" icon="el-icon-search" @click="search"
        >Search</el-button
      >
      <el-button @click="resetSearch">Clear</el-button>
    </el-form>

    <div style="margin-bottom: 20px">
      <el-button type="primary" @click="addRole">Add</el-button>
      <el-button
        type="danger"
        @click="removeRoles()"
        :disabled="selectedRoles.length === 0"
        >Delete all</el-button
      >
    </div>

    <el-table
      border
      stripe
      style="width: 960px"
      v-loading="listLoading"
      :data="roles"
      @selection-change="handleSelectionChange"
    >
      <el-table-column type="selection" width="55" />

      <el-table-column type="index" label="ID" width="80" align="center">
      </el-table-column>

      <el-table-column label="Role Name">
        <template slot-scope="{ row }">
          <template v-if="row.edit">
            <el-input v-model="row.roleName" class="edit-input" size="small" />
            <el-button
              class="cancel-btn"
              size="small"
              icon="el-icon-refresh"
              type="warning"
              @click="cancelEdit(row)"
            >
              Cancel
            </el-button>
          </template>
          <span v-else>{{ row.roleName }}</span>
        </template>
      </el-table-column>

      <el-table-column label="Operation" width="300" align="center">
        <template slot-scope="{ row }">
          <el-button
            size="mini"
            type="info"
            icon="el-icon-info"
            title="Assign Permissions"
            @click="
              $router.push(`/acl/role/auth/${row.id}?roleName=${row.roleName}`)
            "
          ></el-button>
          <el-button
            size="mini"
            type="primary"
            icon="el-icon-check"
            title="Yes"
            @click="updateRole(row)"
            v-if="row.edit"
          ></el-button>
          <el-button
            size="mini"
            type="primary"
            icon="el-icon-edit"
            title="Modify Role"
            @click="row.edit = true"
            v-if="!row.edit"
          ></el-button>
          <el-button
            size="mini"
            type="danger"
            icon="el-icon-delete"
            title="Delete Role"
            @click="removeRole(row)"
          ></el-button>
        </template>
      </el-table-column>
    </el-table>

    <!-- 分页组件 -->
    <el-pagination
      :current-page="page"
      :total="total"
      :page-size="limit"
      :page-sizes="[5, 10, 20, 30, 40, 50, 100]"
      style="padding: 20px 0"
      layout="prev, pager, next, jumper, ->, sizes, total"
      @current-change="getRoles"
      @size-change="handleSizeChange"
    />
  </div>
</template>

<script>
export default {
  name: 'RoleList',

  data() {
    return {
      listLoading: true, // 数据是否正在加载
      roles: [], // 角色列表
      total: 0, // 总记录数
      page: 1, // 当前页码
      limit: 5, // 每页记录数
      tempSearchObj: {
        // 收集搜索条件数据
        roleName: '',
      },
      searchObj: {
        // 发送请求的条件参数数据
        roleName: '',
      },
      selectedRoles: [], // 所有选中的角色列表
    }
  },

  mounted() {
    this.getRoles()
  },

  methods: {
    /* 
    取消修改
    */
    cancelEdit(role) {
      role.roleName = role.originRoleName
      role.edit = false
      this.$message.warning('Cancel the modification?')
    },

    /* 
    更新角色
    */
    updateRole(role) {
      this.$API.role
        .updateById({ id: role.id, roleName: role.roleName })
        .then((result) => {
          this.$message.success(result.message || 'Successfully Update')
          this.getRoles(this.page)
        })
    },

    /* 
    每页数量发生改变的监听
    */
    handleSizeChange(pageSize) {
      this.limit = pageSize
      this.getRoles()
    },

    /* 
    添加角色
    */
    addRole() {
      // 显示添加界面
      this.$prompt('Please enter the role name', 'Add the role', {
        confirmButtonText: 'Yes',
        cancelButtonText: 'Cancel',
      })
        .then(({ value }) => {
          this.$API.role.save({ roleName: value }).then((result) => {
            this.$message.success(result.message || 'Success!')
            this.getRoles()
          })
        })
        .catch(() => {
          this.$message.warning('Canceled!')
        })
    },

    /* 
    异步获取角色分页列表
    */
    getRoles(page = 1) {
      this.page = page
      this.listLoading = true
      const { limit, searchObj } = this
      this.$API.role
        .getPageList(page, limit, searchObj)
        .then((result) => {
          const { items, total } = result.data
          this.roles = items.map((item) => {
            item.edit = false // 用于标识是否显示编辑输入框的属性
            item.originRoleName = item.roleName // 缓存角色名称, 用于取消
            return item
          })
          this.total = total
        })
        .finally(() => {
          this.listLoading = false
        })
    },

    /* 
    根据搜索条件进行搜索
    */
    search() {
      this.searchObj = { ...this.tempSearchObj }
      this.getRoles()
    },

    /* 
    重置查询表单搜索列表
    */
    resetSearch() {
      this.tempSearchObj = {
        roleName: '',
      }
      this.searchObj = {
        roleName: '',
      }
      this.getRoles()
    },

    /* 
    删除指定的角色
    */
    removeRole({ id, roleName }) {
      this.$confirm(`Are you sure to delete '${roleName}' ?`, 'hint', {
        type: 'warning',
      })
        .then(async () => {
          const result = await this.$API.role.removeById(id)
          this.getRoles(this.roles.length === 1 ? this.page - 1 : this.page)
          this.$message.success(result.message || 'Success!')
        })
        .catch(() => {
          this.$message.info('Canceled!')
        })
    },

    /* 
    当表格复选框选项发生变化的时候触发
    */
    handleSelectionChange(selection) {
      this.selectedRoles = selection
    },

    /* 
    批量删除
    */
    removeRoles() {
      this.$confirm('This operation will delete pernamnetly,if continue?', 'hint', {
        type: 'warning',
      })
        .then(async () => {
          const ids = this.selectedRoles.map((role) => role.id)
          const result = await this.$API.role.removeRoles(ids)
          this.getRoles()
          this.$message({
            type: 'success',
            message: 'Success',
          })
        })
        .catch(() => {
          this.$message({
            type: 'info',
            message: 'Canceled',
          })
        })
    },
  },
}
</script>

<style scoped>
.edit-input {
  padding-right: 100px;
}
.cancel-btn {
  position: absolute;
  right: 15px;
  top: 10px;
}
</style>