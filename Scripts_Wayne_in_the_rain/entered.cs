using System.Collections;
using System.Collections.Generic;
using UnityEngine;


public class entered : MonoBehaviour
{
    //Sprite closed = Resources.Load("closedcoffeeshop", typeof(Sprite)) as Sprite;

    bool enter; // boolean to determine whether you are inside the trigger or outside

    void Start()
    {
        enter = false; // before entering the trigger area
    }
    // Update is called once per frame
    void Update()
    {
        if (enter == true && Input.GetKeyDown(KeyCode.Return)) // determine that only after the trigger is
        {
            // true and you press space you can enter the house
            Destroy(this.GetComponent<BoxCollider2D>());
            player.instance.addHealth();
            Debug.Log("You bought a coffee and warmed yourself up!");
            enter = false;
            Debug.Log("The coffeeshop is now closed.");

            //GetComponent(SpriteRenderer).sprite = closed;

        }
    }

    private void OnTriggerEnter2D(Collider2D other)
    {
        if (other.GetComponent<player>() != null)
        {
            enter = true;

        }

         

        
    }

}
