
enum opcode
{
	Code_I2C;

};

//'*'不算进长度
写：
操作码+从机写地址+长度（5）+寄存器地址+值+'*'
应答（重新读取）：
操作码+从机读地址+长度（5）+寄存器地址+值+'*'

读：
操作码+从机读地址+长度（4）+寄存器+'*'
应答：
操作码+从机读地址+长度（5）+寄存器+值+'*'


