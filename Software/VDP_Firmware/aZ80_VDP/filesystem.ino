// file system object from SdFat
FatVolume fatfs;

void filesystem_mount() {
  is_filesystem_mounted = fatfs.begin(&msc_block_dev);

  if (is_filesystem_mounted) {
    #ifdef DEBUG
    DEBUG_PRINT("USB Drive mounted: \r\n");
    fatfs.ls(&Serial2, LS_SIZE);    
    #endif
  }
}

void filesystem_umount() {
  // unmount file system
  is_filesystem_mounted = false;
  DEBUG_PRINT("USB Drive unmounted.\r\n");
  fatfs.end();
}

void filesystem_cmd_sequence_received(char* cmd_str) {
  DEBUG_PRINT("Received CMD Sequence. [%s]\r\n",cmd_str);
  if (strcmp(cmd_str, "dir") == 0 && is_filesystem_mounted) {
        DEBUG_PRINT("Directory Listing:\r\n");
        fatfs.ls(&Serial1, LS_SIZE);
        fatfs.ls(&Serial2, LS_SIZE);  
  }
}
