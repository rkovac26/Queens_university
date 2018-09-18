using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;
using UnityEngine.Audio;


public class player : MonoBehaviour 
{
    //Make sure your GameObject has an AudioSource component first
    AudioSource m_AudioSource;
    //Make sure to set an Audio Clip in the AudioSource component
    AudioClip m_AudioClip;
    //Make sure you set an AudioClip in the Inspector window
    public AudioClip m_AudioClip2;



    public static player instance;
	private Rigidbody2D body;

	private Animator animator;

	/// <summary>
	/// Player Movement
	/// </summary>
	[SerializeField]
	private float speed;
	private bool facingRight;
	private bool umbrella;


    /// <summary>
    /// Is the player jumping, etc.
    /// </summary>
    [SerializeField]
	private Transform[] groundPoints;
	[SerializeField] 
	private float groundRadius;
	[SerializeField]
	private LayerMask isGround;
	private bool grounded;
	private bool jump;
	[SerializeField]
	private float jumpForce;

	/// HEALTH BAR & Damage stuff
	/// Gets or sets the current health.
	/// </summary>
	/// <value>The current health.</value>
	public float currentHealth { get; set;}
	public float maxHealth { get; set;}
	public float newHealth;
	public Slider healthbar;
	public float wetTime = 0.01f;
	private float wetTimer;
	private bool gameOver;
	public GameObject gameOvertext;

	/// <summary>
	/// WIND STUFF
	/// </summary>
	public float windSpeed = 10f;
    public float randomWind;
    private float windTimer;
	private float windTime;
	private float windDuration;
	private float windDurationTimer;
	public bool windRight;
	public bool windLeft;
	private bool windActive;
	public bool inWindArea = false;
	public GameObject windZone;


	void Awake ()
	{
        

        if (instance == null)
		{
			instance = this;
		}   
		else if (instance != this)
		{
			Destroy(gameObject);
		}
	}

	// Use this for initialization
	void Start () 
	{

        //Fetch the AudioSource from the GameObject
        m_AudioSource = GetComponent<AudioSource>();
        //Set the original AudioClip as this clip
        //Output the current clip's length


        gameOver = false;
		umbrella = true; 
		facingRight = true;

		body = GetComponent<Rigidbody2D> ();
		animator = GetComponent<Animator> ();
		///HEALTH
		maxHealth = 100f;
		currentHealth = maxHealth;
		healthbar.value = CalculateHealth();
		///WIND
		windLeft = false;
		windRight = false;
		windActive = false;


    }
	
	// Update is called once per frame
	void Update()
	{


        footsteps();

        HandleInput();
		WaterDamage ();

	}

	void FixedUpdate () 
	{

        

        grounded = onGround ();
		float horizontal = Input.GetAxis ("Horizontal");

        if (!gameOver)
        {
            Movement(horizontal);
            Flip(horizontal);
        }
		


        UmbrellaInput();
		HandleLayers ();
		ResetVals ();
        ///WindTiming ();

        if (Random.value < 0.002)
        {
            Wind();

        }
        ///Debug.Log (windRight);
        if (inWindArea && umbrella == true)
		{
			WindUp();
		}
	}

    IEnumerator windEnumerator()
    {
        print(Time.time);
        yield return new WaitForSecondsRealtime(Random.Range(5, 50));
        print(Time.time);
    }


    private void footsteps()
    {
        if ((Input.GetAxis("Horizontal") < 0) && (onGround() == true))
        {
            GetComponent<AudioSource>().UnPause();

        }
        else if ((Input.GetAxis("Horizontal") > 0) && (onGround() == true))
        {
            GetComponent<AudioSource>().UnPause();

        }
        else
        {
            GetComponent<AudioSource>().Pause();

        }
    }
    private void WaterDamage ()
	{
		if (umbrella == false) {
			wetTimer += Time.deltaTime*0.01f;
		} 
		else 
		{
			wetTimer = 0;
		}
		if (wetTimer > wetTime) 
		{
			DealDamage (50);
			wetTimer = 0;
		}
	}



	private void DealDamage (float damageVal)
	{
		currentHealth -= damageVal;
		healthbar.value = CalculateHealth();
		if (currentHealth <= 0)
		{
			Die();
		}
	}

	public void addHealth ()
	{
		newHealth = (maxHealth - currentHealth)/2;
		DealDamage(-newHealth);
	}

    public bool umbrellaUp()
    {
        return umbrella;
    }
    public void lighteningStruck()
    {
        Die();
    }

    float CalculateHealth()
	{
		return currentHealth / maxHealth;
	}

	

	private void Die()
	{
        currentHealth = 0;
        Debug.Log("dead");

        
		gameOver = true;
        SceneManager.LoadScene("GameOverScreen");
    }


	
	private void Wind ()
	{

        randomWind = Random.value;


        if (randomWind < 0.25)
		{
			windRight = true;

			if (umbrella == true) 
			{
				body.velocity = Vector2.zero;
				body.AddForce (new Vector2 (-windSpeed * 65, 0)); ///setting the speed of the wind
				body.transform.rotation = Quaternion.Euler (0, 0, 25);
            }

		}

		else if (randomWind > 0.75) ///wind is forward
		{
			windLeft = true;
 
			if (umbrella == true) 	
			{
				body.velocity = Vector2.zero;
				body.AddForce (new Vector2 (windSpeed * 50, 0)); ///setting the speed of the wind
				body.transform.rotation = Quaternion.Euler (0, 0, -25);
            }

		} 
		else 
		{
			windLeft = false;
			windRight = false;
			body.transform.rotation = Quaternion.Euler(0,0,0);
        }

	}

	private void WindUp()
	{
		body.velocity = Vector2.zero;
		body.AddForce(windZone.GetComponent<windArea>().direction);
	}

	void OnTriggerEnter2D(Collider2D coll)
	{
		if (coll.gameObject.tag == "windArea")
		{
			windZone = coll.gameObject;
			inWindArea = true;
		}
	}

	void OnTriggerExit2D(Collider2D coll)
	{
		if (coll.gameObject.tag == "windArea")
		{
			inWindArea = false;
		}
	}


	private void Movement(float horizontal)
	{
        
        if (umbrella == true) 
		{
			///body.velocity = new Vector2 (horizontal * speed, body.velocity.y); //x = -1, y = 0
			body.transform.Translate(Vector3.right * speed * Time.deltaTime * horizontal);
			animator.SetFloat ("ismoving", Mathf.Abs (horizontal));
		}
		///when umbrella is down, player moves 20% faster
		if (umbrella == false) 
		{
			///body.velocity = new Vector2 (horizontal * speed*1.2f, body.velocity.y); //x = -1, y = 0
			body.transform.Translate(Vector3.right * speed * 1.2f * Time.deltaTime * horizontal);
			animator.SetFloat ("ismoving", Mathf.Abs (horizontal));
		}

		if (grounded) /// resets gravity
		{
			body.gravityScale = 2f;
		}

		if (body.velocity.y < 0 && umbrella == true && !grounded) ///lowers the amount of gravity
		{
			body.gravityScale = 0.5f;
		}
		if (body.velocity.y < 0 && umbrella == false && !grounded) ///ups the amount of gravity
		{
			body.gravityScale = 3f;
		}

		if (grounded && jump) 
		{
			grounded = false;
			body.gravityScale = 2f;
			body.AddForce(new Vector2(0, jumpForce));
			animator.SetTrigger ("jump");
		}

	}

	private void UmbrellaInput ()
	{
		if (umbrella == true) {
			animator.SetBool ("umbrella", true);
		} else 
		{
			animator.SetBool ("umbrella", false);
		}
	}

	private void HandleInput()
	{
		if (Input.GetKeyDown (KeyCode.Space)) 
		{
			jump = true;
		}
		if (Input.GetKeyDown (KeyCode.LeftShift)) 
		{
			umbrella = !umbrella;
		}
		
	}

	private void Flip(float horizontal)
	{
		if (horizontal > 0 && !facingRight || horizontal < 0 && facingRight) 
		{

            m_AudioSource.Play();

            facingRight = !facingRight;

			Vector3 scale = transform.localScale;
			scale.x *= -1;
			transform.localScale = scale;

            


        }




    }




    private bool onGround()
	{
		if (body.velocity.y <= 0)
		{
			foreach (Transform point in groundPoints)
			{
				Collider2D [] colliders = Physics2D.OverlapCircleAll(point.position,groundRadius,isGround);

				for (int i = 0; i < colliders.Length; i++)
				{
					if (colliders[i].gameObject != gameObject)
					{
						animator.ResetTrigger ("jump");
						animator.SetBool ("land", false);
						return true;
					}
				}
			}
		}
		return false;
	}

	private void ResetVals()
	{
		jump = false;
		grounded = false;
	}

	private void HandleLayers()
	{
		if (!grounded) {
			animator.SetLayerWeight (1, 1);
		} else {
			animator.SetLayerWeight (1, 0);
		}
	}
}
