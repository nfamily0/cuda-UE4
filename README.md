# cuda_rendering

Developed with Unreal Engine 4



CUDA로 제작된 Volume Rendering 프로그램을 lib로 빌드후 Unreal에서 호출한 프로젝트



![캡처](./ReadmeImage/test.gif)



Dynamic Material을 이용하여 CUDA를 통해 생성된 Texture를 동적으로 적용한다.

CUDA는 매 프레임 마다 호출되며 약 33ms의 성능을 보인다. (AMD 5900X, RTX 3060)



Unreal 카메라의 위치 -> CUDA Volume Rendering 카메라위치 

언리얼 내부의 카메라의 위치의 따라 관찰 방향이 변한다.



시간의 흐름의 따른 함수 -> CUDA Volume Rendering Transfer function

관찰되는 물체의 색상이 시간의 흐름의 따라 변한다.
