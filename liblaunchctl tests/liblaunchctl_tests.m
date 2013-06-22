//
//  liblaunchctl_tests.m
//  liblaunchctl tests
//
//  Created by Evan Lucas on 6/21/13.
//  Copyright (c) 2013 Hattiesburg Clinic. All rights reserved.
//

#import <SenTestingKit/SenTestingKit.h>
#import "liblaunchctl.h"

@interface liblaunchctl_tests : SenTestCase
@end

@implementation liblaunchctl_tests

- (void)setUp
{
  [super setUp];
}

- (void)tearDown
{
  // Tear-down code here.
  
  [super tearDown];
}

- (void)testlistAll
{
  jobs_list_t s = launchctl_list_jobs();
  if (s == NULL) {
    STFail(@"An error occurred fetching all jobs %d: %s\n", errno, strerror(errno));
  }
  int count = s->count;
  printf("Found %d jobs\n", count);
  for (int i=0; i<count; i++) {
    launch_data_status_t q = &s->jobs[i];
    printf("Job: %s\t PID: %d\t Status: %d\n", q->label, q->pid, q->status);
  }
  jobs_list_free(s);
}

- (void)list:(NSString *)label {
  
}
- (void)testExample
{
  
}

@end
