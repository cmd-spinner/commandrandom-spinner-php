# 1. Is Database Trusted/Untrusted?

By default, a database is considered as an untrusted source. Only when there is no untrusted data going into the database, the database is considered trusted.

# 2. Second order SQL injection Example
We show an example of the second order SQL injection vulnerability and how SPINNER mitigates it. 
Note that this is a hypothetical example for presentation purpose. The application is not currently known to be vulnerable to this particular second order SQL injection attack.

## Outline of the program
The below code snippets are from the PHPSHE program (s5) which is an ecommerce application. 

* User Registration 
	* (A): During user registration, the untrusted source `_POST["user_name"]` passes value to `$_p_user_name`.
	* (B) Then, `$_p_user_name` is passed to `$sql_set`.
	* (C) Later, `$sql_set` is inserted to the table `user` by `$db->pe_insert()`.


```php 
/***********************************************/
/* User Registration                           */
/***********************************************/
switch($act) { 
  case 'register':
    $_p_user_name = $_POST["user_name"];                    // ---------(A)--------- 
    $sql_set['user_name'] = $_p_user_name;                  // ---------(B)--------- 
    
    // user registration, data from untrusted sources are added to the database
    $user_id = $db->pe_insert('user', pe_dbhold($sql_set)); // ---------(C)---------
    $db->pe_insert('user', pe_dbhold($sql_set);
    
    //user registration process finished. callback function is invoked.
    user_login_callback('reg', $user_id);
}
``` 

* User Login
	* (D) After user registration, `user_login_callback()` is invoked to update user information for user reference
	* (E) It uses `$db->select` to fetech user names from database and store it in variable `$tguser`. 
	* (F) `tguser` then is passed to `$sql_set`. 
	* (G) Finally, `pe_update()` is called to update the user reference relationship.

```php 
/***********************************************/
/* User Login                                  */
/***********************************************/
function user_login_callback($type, $user) { // ---------(D)--------- 
  $tguser = $db->pe_select('user', array('user_id'=>intval($_COOKIE['tguser_id'])), 'user_id, user_name'); // ---------(E)---------
  $sql_set['tguser_name'] = $tguser['user_name']; // ---------(F)---------
  $db->pe_update('user', array('user_id'=>$user['user_id']), pe_dbhold($sql_set)); // ---------(G)---------
}

public function pe_update($table, $where, $set){
	$sqlset = $this->_doset($set);
	$sqlwhere = $this->_dowhere($where);
	return $this->sql_update("update `".dbpre."{$table}` {$sqlset} {$sqlwhere}");
}

protected function _doset($set){
	if (is_array($set) && count($set, 1) > count($set)) {
		foreach ($set as $set_one) {
			$key_arr = $val_arr = array();
			foreach ($set_one as $k => $v) {
				$key_arr[] = str_ireplace('`', '', $k);
				$val_arr[] = "'{$v}'"; // ---------(H)---------
			}
			$val_str[] = "(" . implode($val_arr, ', ') . ")";
		}
		$key_str = "(" . implode($key_arr, ', ') . ")";
		$sqlset = "{$key_str}  values ".implode($val_str, ', ');
	}
	elseif (is_array($set) && count($set, 1) == count($set)) {
		foreach ($set as $k => $v) {
			$k = str_ireplace('`', '', $k);
			$set_arr[] = "`{$k}` = '{$v}'";
		}
		$sqlset = 'set '.implode($set_arr, ' , ');
	}
	else {
		$sqlset = "set {$set}";
	}
	return $sqlset;
}
```

## Exploit
1. In the above example, an attacker can try to execute second order SQL injection through `$_p_user_name`. 
2. Since there is a data-flow from untrusted sources `_POST[...]` to the database, database is **untrusted**. 
3. Hence, the value fetched from the database **will not be considered trusted** and will not be randomized. 
4. The content of `$sql_set` (`$sqlset` in `_doset()`) will be quoted in `_doset()` (See (H)). 
5. If an injected query escapes the quote, Spinner's scanner will recognize and derandomize the injected query, preventing the second order SQL injection.
