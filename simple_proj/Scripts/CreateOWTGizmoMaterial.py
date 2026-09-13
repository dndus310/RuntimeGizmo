"""Prepare the static material permutation used by runtime gizmo MIDs."""

import unreal


def create_gizmo_material():
    parent_path = '/Engine/InteractiveToolsFramework/Materials/GizmoComponentMaterial_NotDimmed'
    package_path = '/Game/VTBOWT/Materials'
    asset_name = 'MI_OWTGizmo_NotOccluded'
    asset_path = package_path + '/' + asset_name
    parent = unreal.load_asset(parent_path)
    if not isinstance(parent, unreal.MaterialInstanceConstant):
        raise RuntimeError('The engine gizmo material instance is missing: ' + parent_path)

    library = unreal.MaterialEditingLibrary
    switches = [str(name) for name in library.get_static_switch_parameter_names(parent)]
    if 'OccludeByCustomDepth' not in switches:
        raise RuntimeError('The engine material has no OccludeByCustomDepth static switch.')

    instance = unreal.load_asset(asset_path) if unreal.EditorAssetLibrary.does_asset_exist(asset_path) else None
    if instance is None:
        instance = unreal.AssetToolsHelpers.get_asset_tools().create_asset(
            asset_name, package_path, unreal.MaterialInstanceConstant,
            unreal.MaterialInstanceConstantFactoryNew())
    if not isinstance(instance, unreal.MaterialInstanceConstant):
        raise RuntimeError('Unable to create gizmo material instance: ' + asset_path)
    if instance.get_editor_property('parent') not in (None, parent):
        raise RuntimeError('Existing gizmo material has a different parent: ' + asset_path)

    library.set_material_instance_parent(instance, parent)
    # UE 5.7 returns false even after applying this setter; verify the effective value below.
    library.set_material_instance_static_switch_parameter_value(instance, 'OccludeByCustomDepth', False)
    library.update_material_instance(instance)
    if library.get_material_instance_static_switch_parameter_value(instance, 'OccludeByCustomDepth'):
        raise RuntimeError('OccludeByCustomDepth must be disabled.')
    if not unreal.EditorAssetLibrary.save_loaded_asset(instance):
        raise RuntimeError('Unable to save gizmo material instance.')
    unreal.log('OWT_GIZMO_MATERIAL: PASS ' + asset_path)


create_gizmo_material()
