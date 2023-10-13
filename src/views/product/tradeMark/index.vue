<template>
	<div>
		<el-input placeholder="Please enter the content" v-model="keyword" clearable @change="keywordChange"
			style="width: 200px; padding-right: 10px">
		</el-input>
		<el-button type="primary" icon="el-icon-search" @click="search">Search</el-button>
		<!-- 按钮 -->
		<el-button type="primary" icon="el-icon-plus" style="margin-bottom: 10px; margin-top: 10px" @click="showDialog">
			Add</el-button>
		<!-- 表格组件 -->
		<!-- 以列展示数据 -->
		<el-table :data="list" style="width: 100%" border>
			<el-table-column type="index" label="ID" width="80px" align="center">
			</el-table-column>
			<el-table-column prop="tmName" label="Robot Name" width="width">
			</el-table-column>
			<!--  -->
			<el-table-column label="Robot Picture" width="width">
				<template slot-scope="{ row }">
					<img v-lazy="row.logoUrl" style="width: 100px" />
				</template>
			</el-table-column>
			<el-table-column label="Operation" width="width">
				<template slot-scope="{ row }">
					<el-button type="warning" size="mini" icon="el-icon-edit" @click="updateTradeMark(row)">Modify
					</el-button>
					<el-button type="danger" size="mini" icon="el-icon-delete" @click="deleteTradeMark(row)">Delete
					</el-button>
				</template>
			</el-table-column>
		</el-table>
		<!-- 
      current-page：当前页数
      page-sizes：每页显示个数选择器
      page-size：每页显示条目个数
      total：总条目数
      pager-count：页码按钮的数量，当总页数超过该值时会折叠
      layout：实现分页器布局
    -->
		<el-pagination class="pagination" :current-page="page" :total="total" :page-size="limit" :pager-count="7"
			:page-sizes="[3, 5, 10]" layout="prev, pager, next, jumper, ->, sizes, total"
			@current-change="handleCurrentChange" @size-change="handleSizeChange">
		</el-pagination>
		<!-- Form 组件提供了表单验证的功能，只需要通过 rules 属性传入约定的验证规则，
        并将 Form-Item 的 prop 属性设置为需校验的字段名即可。 -->
		<!-- 对话框 
    :visible.sync：控制对话框显示与隐藏
    -->
		<el-dialog :title="tmForm.id ? 'Modify the name' : 'Add the name'" :visible.sync="dialogFormVisible">
			<!-- :model属性将表单数据收集到一个指定对象上 -->
			<el-form style="width: 80%" :model="tmForm" :rules="rules" ref="ruleForm">
				<el-form-item label="Name" label-width="100px" prop="tmName">
					<el-input autocomplete="off" v-model="tmForm.tmName"></el-input>
				</el-form-item>
				<el-form-item label="Picture" label-width="100px" prop="logoUrl">
					<!-- 上传 不能使用v-model收集数据-->
					<el-upload class="avatar-uploader" action="/dev-api/admin/product/fileUpload"
						:show-file-list="false" :on-success="handleAvatarSuccess" :before-upload="beforeAvatarUpload">
						<img v-if="tmForm.logoUrl" :src="tmForm.logoUrl" class="avatar" />
						<i v-else class="el-icon-plus avatar-uploader-icon"></i>
						<div slot="tip" class="el-upload__tip">
							Only to upload jpg/png, which less than 2MB
						</div>
					</el-upload>
				</el-form-item>
			</el-form>
			<div slot="footer" class="dialog-footer">
				<el-button @click="dialogFormVisible = false">Cancel</el-button>
				<el-button type="primary" @click="addOrUpdateTradeMark">Yes</el-button>
			</div>
		</el-dialog>
	</div>
</template>

<script>
	export default {
		name: 'tradeMark',
		data() {
			return {
				//搜索关键字
				keyword: '',
				//当前页
				page: 1,
				//当前页数据条数
				limit: 3,
				//总共多少数据
				total: 0,
				// 列表展示的数组
				list: [],
				//对话框显示与隐藏的属性控制
				dialogFormVisible: false,
				//收集的品牌信息
				tmForm: {
					tmName: '',
					logoUrl: '',
				},
				//表单验证规则
				rules: {
					tmName: [
						//required：必须验证的字段，message：提示信息，trigger：用户行为，blur失焦，change发生改变
						{
							required: true,
							message: 'Please enter the brand name',
							trigger: 'blur'
						},
						{
							min: 2, //字符长度
							max: 10,
							message: 'The length is between 2 to 10 character',
							trigger: 'change',
						},
					],
					//图片格式、大小不在这里验证，因为他不是表单元素， 在beforeAvatarUpload方法中有校验规则
					logoUrl: [{
						required: true,
						message: 'Please upload the Picture'
					}],
				}
			}
		},
		mounted() {
			this.getPageList()
		},
		methods: {
			//获取商品列表
			async getPageList() {
				const {
					page,
					limit
				} = this
				let result = await this.$API.tradeMark.reqTradeMarkList(page, limit)
				if (result.code == 200) {
					this.total = result.data.total
					this.list = result.data.records
				}
			},
			//点击分页器跳转页码
			handleCurrentChange(pager) {
				this.page = pager
				this.change()
			},
			//展示数据条数切换
			handleSizeChange(sizes) {
				this.limit = sizes
				this.change()
			},
			//点击添加显示对话框
			showDialog() {
				this.dialogFormVisible = true
				this.tmForm = {
					tmName: '',
					logoUrl: ''
				}
			},
			//点击修改按钮
			updateTradeMark(row) {
				this.dialogFormVisible = true
				/* 
				这里不用浅拷贝的话，当你在弹出框更改品牌名称的时候，
				品牌列表展示页面会自动的更新你修改的数据—无论你点了【取消】还是【确定修改】
				*/
				this.tmForm = {
					...row
				}
			},
			//element-UI图片相关回调
			//图片上传成功
			handleAvatarSuccess(res, file) {
				//res：上传成功之后返回的前端数据
				//file：同上
				//收集品牌图片数据
				this.tmForm.logoUrl = res.data
			},
			//图片上传之前
			beforeAvatarUpload(file) {
				const isJPG = file.type === 'image/jpeg'
				const isLt2M = file.size / 1024 / 1024 < 2

				if (!isJPG) {
					this.$message.error('Only JPG format!')
				}
				if (!isLt2M) {
					this.$message.error('Must less than 2MB!')
				}
				return isJPG && isLt2M
			},
			//添加或修改品牌信息
			addOrUpdateTradeMark() {
				//当全部字段验证通过在执行操作
				this.$refs.ruleForm.validate(async (valid) => {
					if (valid) {
						this.dialogFormVisible = false
						//发请求
						let result = await this.$API.tradeMark.reqAddOrUpdateTradeMark(
							this.tmForm
						)
						if (result.code == 200) {
							//element-ui的消息提示
							this.$message({
								message: this.tmForm.id ? 'Success to modify' : 'Success to add',
								type: 'success',
							})
							//判断是修改还是添加操作，修改则留在当前页，添加返回第一页
							this.handleCurrentChange(this.tmForm.id ? this.page : 1)
							this.getPageList()
						}
					} else {
						this.$message.error('The information is not clear')
						return false
					}
				})
			},
			//删除品牌操作
			deleteTradeMark(row) {
				this.$confirm(`Sure to delete${row.tmName}?`, 'hint', {
						confirmButtonText: 'Yes',
						cancelButtonText: 'Cancel',
						type: 'warning',
					})
					//点击确定触发
					.then(async () => {
						let result = await this.$API.tradeMark.reqDeleteTradeMark(row.id)
						if (result.code == 200) {
							this.$message({
								type: 'success',
								message: 'Success!',
							})
							this.handleCurrentChange(
								this.list.length > 1 ? this.page : this.page - 1
							)
							this.change()
						}
					})
					//点击取消触发
					.catch(() => {
						this.$message({
							type: 'info',
							message: 'Canceled',
						})
					})
			},
			//获取搜索列表
			async searchData() {
				const arr = []
				let result = await this.$API.tradeMark.reqSearchTradeMark(this.keyword)
				if (result.code == 200) {
					for (let i = 0; i < result.data.length;) {
						arr.push(result.data.slice(i, (i += this.limit)))
					}
					this.list = arr[this.page - 1]
					this.total = result.data.length
				}
			},
			search() {
				if (this.keyword == '') {
					this.$message.error('Input cannot be empty')
				} else {
					this.searchData()
				}
			},
			change() {
				if (this.keyword == '') {
					this.getPageList()
				} else {
					this.searchData()
				}
			},
			keywordChange() {
				this.change()
			},
		},
	}
</script>

<style>
	.pagination {
		margin-top: 20px;
		text-align: center;
	}

	.avatar-uploader .el-upload {
		border: 1px dashed #d9d9d9;
		border-radius: 6px;
		cursor: pointer;
		position: relative;
		overflow: hidden;
	}

	.avatar-uploader .el-upload:hover {
		border-color: #409eff;
	}

	.avatar-uploader-icon {
		font-size: 28px;
		color: #8c939d;
		width: 178px;
		height: 178px;
		line-height: 178px;
		text-align: center;
	}

	.avatar {
		width: 178px;
		height: 178px;
		display: block;
	}
</style>
