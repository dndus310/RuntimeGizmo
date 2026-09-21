#pragma once

class USceneComponent;
class UTransformProxy;

namespace UE::VTBOWTEditor
{
	void AddActorTransformTarget(UTransformProxy& Proxy, USceneComponent* RootComponent, USceneComponent* FrameComponent);
}
