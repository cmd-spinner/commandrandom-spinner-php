<?php

namespace plainview\sdk_pvam\object_stores;

use \Exception;

/**
	@brief		Main class for handling stores.
	@since		2016-01-02 01:00:09
**/
trait Store
{
	/**
		@brief		Return the container that stores this object.
		@since		2015-10-23 10:54:49
	**/
	public static function store_container()
	{
		throw new Exception( 'Please override the store_container method.' );
	}

	/**
		@brief		Delete the object completely.
		@since		2015-10-23 10:54:49
	**/
	public static function delete()
	{
		throw new Exception( 'Please override the delete_from_store method.' );
	}

	/**
		@brief		Return the storage key.
		@details	Key / ID.
		@since		2016-01-02 01:03:18
	**/
	public static function store_key()
	{
		throw new Exception( 'Please override the store_key method.' );
	}

	/**
		@brief		Load the object from the store.
		@since		2015-10-22 22:16:03
	**/
	public static function load()
	{
		// Conv
		$container = static::store_container();

		$key = static::store_key();
		$__key = '__' . $key;

		// Does the object already exist in the container cache?
		if ( isset( $container->$__key ) )
			return $container->$__key;

		// Try to load the object from the store.
		$r = static::load_from_store( $key );

		$r = maybe_unserialize( $r );
		if ( ! is_object( $r ) )
		{
			// Backwards compatability: The Options_Object used to base64 encode the object.
			$r = base64_decode( $r );
			$r = maybe_unserialize( $r );
			// If there is still no object, just create a new one.
			if ( ! is_object( $r ) )
				$r = new static();
		}

		// Save to the cache.
		$container->$__key = $r;

		return $r;
	}

	/**
		@brief		Internal method to try to load the object from the store.
		@since		2016-01-02 01:15:39
	**/
	public static function load_from_store( $key )
	{
		throw new Exception( 'Please override the load_from_store method.' );
	}

	/**
		@brief		Save the object to the store.
		@since		2016-01-02 01:29:20
	**/
	public function save()
	{
		throw new Exception( 'Please override the store_key method.' );
	}

}
