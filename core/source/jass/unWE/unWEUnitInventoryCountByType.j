#ifndef unWEUnitInventoryCountByTypeIncluded
#define unWEUnitInventoryCountByTypeIncluded

//===========================================================================
//��λ - ������Ʒ������ָ�����ͣ� 
//===========================================================================
library unWEUnitInventoryCountByType
 
function unWEUnitInventoryCountByType takes unit whichUnit,integer itemID returns integer
    local integer index = 0
    local integer count = 0

    loop
        if (GetItemTypeId(UnitItemInSlot(whichUnit, index)) == itemID) then
            set count = count + 1
        endif

        set index = index + 1
        exitwhen index >= bj_MAX_INVENTORY
    endloop

    return count
endfunction
 
endlibrary 
#endif /// unWEUnitInventoryCountByTypeIncluded
