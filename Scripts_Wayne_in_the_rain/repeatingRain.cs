using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class repeatingRain : MonoBehaviour {
	
	public GameObject player;
	private float verticalSize;
	private float horizontalPosn;
	private float playerXPosn;
	// Use this for initialization

	void Start () {
		verticalSize = 12f;
		horizontalPosn = 15f;
	}
	
	// Update is called once per frame
	void Update () 
	{
		if (transform.position.y < -verticalSize) 
		{
			RepositionRainVert ();
		}
		if (player.transform.position.x > horizontalPosn) 
		{
			RepostionRainHor ();
		}
		if (player.transform.position.x < horizontalPosn-20f) 
		{
			RepostionRainHor2 ();
		}

	}

	private void RepositionRainVert ()
	{
		Vector2 rainOffset = new Vector2 (0,verticalSize * 2f);
		transform.position = (Vector2)transform.position + rainOffset;
	}

	private void RepostionRainHor ()
	{
		Vector2 rainHorz = new Vector2 (20f, 0);
		horizontalPosn = horizontalPosn + 20f;
		transform.position = (Vector2)transform.position + rainHorz;
		Debug.Log (transform.position);
	}
	private void RepostionRainHor2 ()
	{
		Vector2 rainHorz = new Vector2 (20f, 0);
		horizontalPosn = horizontalPosn - 20f;
		transform.position = (Vector2)transform.position - rainHorz;
	}

}
