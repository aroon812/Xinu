	.text
	.globl	test_and_set

/**
 * test_and_set: assume it's called with test_and_set(&lock);
 * @param lock pointer to a mutex_t variable
 *
 * TODO - add a comment each line describing its action
 */
test_and_set:
	pushl	%ebx		# preserve contents of the %ebx register, which is about to be modified
	movl	8(%esp), %ebx	# store content of 8(%esp) into %ebx
	movl	$1, %eax	# store value 1 to %eax register
	xchg	%eax, (%ebx)	# exchange %eax with %ebx
	popl	%ebx		# pop back original content of %ebx register
	ret			# return to calling procedure and returns original value of ebx
