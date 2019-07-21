import functools, os, sys

signatures = [ (b"\x33\xF6\x80\xBD\x21\xFF\xFF\xFF\x01\x76\x05\x83\xCB\xFF", [8, 22])
             , (b"\x80\xBD\x22\xFF\xFF\xFF\x01\x8B\xD8\x76", [6])
             , (b"\x80\xBD\x24\xFF\xFF\xFF\x01\x76", [6])
             , (b"\xC7\x85\xF8\xFE\xFF\xFF\x01\x00\x00\x00\x80\xBD\x21\xFF\xFF\xFF\x01\x76", [16])
             , (b"\x8B\xB5\xF8\xFE\xFF\xFF\x80\xBD\x23\xFF\xFF\xFF\x01\x0F\x86", [12])
             , (b"\x0F\xB6\xC8\x8B\x45\x10\x80\x3C\x01\x01\x77", [9])
             ]
     
originalConstant = 0x01        
replacementConstant = 0x7F

def findPatchLocation(exeContents, signature):
    pos = exeContents.find(signature[0])
    if pos == -1:
        patchedSignature = bytearray(signature[0])
        for i in signature[1]:
            if i < len(patchedSignature):
                patchedSignature[i] = replacementConstant
        if exeContents.find(patchedSignature) != -1:
            pos = -2
            
    return pos

def findPatchLocations(exeContents):
    return list(map(functools.partial(findPatchLocation, exeContents), signatures))
   
def patchBinary(exeContents, patchLocations): 
    for (_, positions), start in zip(signatures, patchLocations):
        if start != -2:
            for offset in positions:
                if exeContents[start + offset] not in [originalConstant, replacementConstant]:
                    return False
                exeContents[start + offset] = replacementConstant
                
    return True

def main():
    if len(sys.argv) != 2:
        print('Usage: aoe2_scroll_bugfix.py <path to AoK HD.exe>')
        return
        
    fileName = sys.argv[1]
    
    print('Reading file...')
    f = open(fileName, 'rb')
    exeContents = bytearray(f.read())
    f.close()
    print("done.\n")
    
    print('Searching for patch signatures...')
    patchLocations = findPatchLocations(exeContents)
    if -1 in patchLocations:
        print("failed! Some signatures are not found!\n")
        return
    print("done.\n")
    
    alreadyPatchedCount = patchLocations.count(-2)
    if alreadyPatchedCount > 0:
        print("Note: %d of %d positions are already patched.\n" % (alreadyPatchedCount, len(patchLocations)))
    
    if alreadyPatchedCount < len(patchLocations):
        print('Patching binary...')
        if not patchBinary(exeContents, patchLocations):
            print("failed! Invalid value found during patching!\n")
            return
        print("done.\n")
        
        backupFileName = fileName + '.backup'    
        print("Backing up original file %s as %s" % (fileName, backupFileName))
        os.rename(fileName, backupFileName)
        print("done.\n")
        
        print('Saving patched binary...')
        f = open(fileName, 'wb')
        f.write(exeContents)
        f.close()
        print("done.\n")
    
    else:
        print("Nothing to do exiting...")
    
main()
