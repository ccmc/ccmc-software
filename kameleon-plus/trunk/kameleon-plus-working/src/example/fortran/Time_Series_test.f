      program time_series_test
c     This program computes
 
c     Three functions used to interpolate 
c     data from a specified batsrus cdf file
 
      external f_kameleon_create, f_kameleon_open, f_kameleon_delete
      external f_kameleon_load_variable, f_kameleon_close
      external f_interpolator_create
      external f_interpolator_delete
      external f_kameleon_interpolate
      external f_kameleon_interpolate_by_id
      external f_kameleon_get_variable_id
 
c     Variables to be used for interpolation and data extraction

      character*300 cdf_file_path
      character*300 directory_containing_cdf_files, output_file
      character*300 list_of_cdf_files 
      character*100 c0,c1,c2
      character*100 variable, list_of_variables

c     r, lat, mlt for iono data
      real rc0, rc1, rc2
      real d0,d1,d2
      real interpolated_value
      REAL, DIMENSION(:), ALLOCATABLE :: interpolated_variables
      integer status,kid,iid, vid
      integer argc,zero, variable_count, var_len, var_shift
      INTEGER io,files_to_read, filecount
      character*80 filecounter, current_file
      zero = 0
 

      argc = iargc()
      variable_count = argc - 6

      if (argc.lt.7) then
        write(*,*) "time_series_test <list_of_cdf_files> ",
     c          "<input_directory> <output_file> ",
     c          " var1 var2 var3 .. c0 c1 c2"
        print *,"    Adapt3D, OpenGGCM, BATSRUS: x y z"
        print *,"    ENLIL, MAS: r theta(latitude) phi(longitude)"
        print *,"    SWMF Iono: 0 theta(latitude) phi(MLT)"
       call exit(1)
      endif

      write(*,*) "Number of variables to interpolate:", variable_count
      ALLOCATE(interpolated_variables(variable_count))

      call getarg(1, list_of_cdf_files)
      call getarg(2, directory_containing_cdf_files)
      call getarg(3, output_file)

c------ Create string for input variables
      var_shift = 1
      DO i = 1, variable_count
        call getarg(3+i, variable)
        var_len = len(trim(variable))
        list_of_variables(var_shift:) = ' '//
     c   trim(variable)
        var_shift = var_shift + var_len + 2
      END DO
      write(*,*) 'Variables to interpolate:', list_of_variables


c----- Get input position and cast to real      
      call getarg(4+variable_count, c0)
      call getarg(5+variable_count, c1)
      call getarg(6+variable_count, c2)

      read(c0,*) rc0
      read(c1,*) rc1
      read(c2,*) rc2

      print *, 'c0: ', rc0, ' c1: ', rc1, ' c2: ', rc2

c----- go through the file list and determine how many timesteps exist
      filecount = 0
      WRITE(*,*) 'filelist =', list_of_cdf_files
      open(unit=81, file=list_of_cdf_files, status='old')
      DO
       READ(81,*,IOSTAT=io) filecounter
       IF (io > 0) THEN
         WRITE(*,*) 'something went wrong, full stop!'
         CLOSE(81)
         STOP
       ELSE IF (io < 0) THEN
         WRITE(*,*) 'Found end of file'
         CLOSE(81)
         EXIT
       ELSE
         filecount = filecount + 1
       END IF
      END DO
      WRITE(*,*) 'files to read from ', 
     c            trim(directory_containing_cdf_files),':',filecount
  
c     Reopen file list
      open(unit=81, file=list_of_cdf_files, status='old')

c-----Open output_file to save results  
      open(unit=82, file=output_file, status='unknown') 
      write(82,*) 'position:', rc0, rc1, rc2
      write(82,*) list_of_variables

c----- Go through file list  
      DO i = 1,filecount
        READ(81,*) current_file
        cdf_file_path=trim(directory_containing_cdf_files)//
     c                current_file
        WRITE(*,*) 'opening ', cdf_file_path

        ! kid will be set when calling f_kameleon_create. 
        kid=0 
        vid=0
  
        call f_kameleon_create(kid) ! multiple kameleon objects can be created 

        call f_kameleon_open(kid,cdf_file_path,status)

        print *, 'done opening file', 'variable count:', variable_count
        DO j = 1, variable_count
          call getarg(3+j, variable)
          call f_kameleon_load_variable(kid,variable)
          print *, 'loaded variable ', variable
        END DO

        call f_interpolator_create(kid, iid)
        write(*,*) 'created interpolator'

        DO j = 1, variable_count
          call getarg(3+j, variable)
          call f_kameleon_get_variable_id(kid,variable, vid);
          call f_kameleon_interpolate_by_id(iid, vid, rc0, rc1, rc2,
     &         d0, d1, d2, interpolated_value)
          print *, 'interpolated value: ', interpolated_value
          interpolated_variables(j) = interpolated_value
          write(82,'(ES14.7,X)', advance='no') interpolated_value
        END DO

        write(82,*) '' !inserts newline

        ! save variables to output_file    
c        WRITE(82,5) (interpolated_variables(j), j = 1, variable_count)

        call f_interpolator_delete(iid);
        call f_kameleon_close(kid)
        call f_kameleon_delete(kid,status)


      END DO  !End file loop

5     FORMAT(ES14.7)
      CLOSE(81) !close list_of_cdf_files
      CLOSE(82) !close output_file




      stop
      end program time_series_test
      
