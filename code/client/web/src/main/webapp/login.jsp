<html>
<head>
<meta http-equiv="content-type" content="text/html; charset=UTF-8">
<title>login</title>
</head>

<body>
	<form action="/resources/j_spring_security_check" method="POST">
		<table>
			<tr>
				<td>User:</td>
				<td><input type='text' id='j_username'  name='j_username' />
				</td>
			</tr>
			<tr>
				<td>Password:</td>
				<td><input type='password' id='j_password' name='j_password'/>
				</td>
			</tr>

			<tr>
				<td colspan='2'><input name="submit" type="submit"
					value="Log In">
				</td>
			</tr>
			<tr>
				<td colspan='2'><input name="reset" type="reset">
				</td>
			</tr>
		</table>
	</form>
</body>
</html>