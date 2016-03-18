using UnityEngine;

namespace assets.Scripts.Utility
{
    public class MoveToMainCamera : MonoBehaviour
    {
        [Header("This objects will move to MainCamera in runtime")]
        public GameObject[] GameObjects = new GameObject[0];

        // Use this for initialization
        void Start()
        {
            var mainCamera = GameObject.FindGameObjectWithTag(Tags.MainCamera);
            if (mainCamera!=null)
            {
                foreach (var gameObj in GameObjects)
                {
                    gameObj.transform.SetParent(mainCamera.transform, true);
                }
            }
            else
            {
                Debug.LogError("Can not find MainCamera game object");
            }
        }

        // Update is called once per frame
        void Update()
        {

        }
    }
}
