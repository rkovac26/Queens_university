using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class WindMovement : MonoBehaviour 
{
	public GameObject player;

	private Rigidbody2D body;
	[SerializeField]
	public float windSpeed = 100000f;
	private Vector3 posn;
	private bool windL;
	private bool windR;
	private Vector3 offset;

	// Use this for initialization
	void Start () {
		body = GetComponent<Rigidbody2D> ();
		posn = transform.position;
	}
	
	// Update is called once per frame
	void FixedUpdate ()
	{
		moveWind ();
	}

	private void moveWind ()
	{
		
		windR = player.GetComponent<player> ().windRight;
		windL = player.GetComponent<player> ().windLeft;

		if (windR == true)
		{
			
			///body.velocity = new Vector2 (-windSpeed, body.velocity.x);
			if (body.transform.position.x > player.transform.position.x-12f) 
			{
				body.transform.Translate (Vector3.right * -windSpeed * Time.deltaTime);
			}
		}
		else if (windL == true) 
		{
			///body.velocity = new Vector2 (windSpeed, body.velocity.y);
			if (body.transform.position.x < player.transform.position.x + 20f) 
			{
				body.transform.Translate (Vector3.right * windSpeed * Time.deltaTime);
			}
		} 
		else 
		{
			transform.position = new Vector3 (player.transform.position.x + posn.x, 0.5f, player.transform.position.z + posn.z);
			///transform.position.x = posn.x + player.transform.position.x;
		}
	}
}
