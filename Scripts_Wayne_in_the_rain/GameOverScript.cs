using System.Collections;
using System.Collections.Generic;
using UnityEngine.SceneManagement;
using UnityEngine;

public class GameOverScript : MonoBehaviour {

    // Use this for initialization
    public void PlayGame()
    {
        SceneManager.LoadScene("Wayne In The Rain - 1");
    }
    public void QuitGame()
    {
        SceneManager.LoadScene("OpeningScene");
    }

}
